extern crate glutin_window;
extern crate graphics;
extern crate opengl_graphics;
extern crate piston;

use glutin_window::GlutinWindow;
use opengl_graphics::{GlGraphics, OpenGL};
use piston::event_loop::*;
use piston::input::*;
use piston::window::WindowSettings;

use rand::thread_rng;
use rand::Rng;
use std::collections::LinkedList;
use std::iter::FromIterator;

#[derive(Clone, PartialEq)]
enum Direction {
    Right,
    Left,
    Up,
    Down,
}

struct Game {
    gl: GlGraphics,
    snake: Snake,
    rows: u32,
    cols: u32,
    tile_width: u32,
    food: Food,
    snake_just_ate: bool,
    score: u32,
}

impl Game {
    fn render(&mut self, args: &RenderArgs) {
        let black: [f32; 4] = [0.0, 0.0, 0.0, 1.0];

        self.gl
            .draw(args.viewport(), |_c, gl| graphics::clear(black, gl));

        self.snake.render(&mut self.gl, args, self.tile_width);

        self.food.render(&mut self.gl, args, self.tile_width);
    }

    fn update(&mut self) -> bool {
        if !self.snake.update(self.snake_just_ate, self.rows, self.cols) {
            return false;
        }

        // The timing of this is a bit odd, b/c technically the snake ate
        // during the previous update but b/c we check for the collision
        // after updating the snake we need to do this check b/4 and
        // after the food update...
        if self.snake_just_ate {
            self.score += 1;
            self.snake_just_ate = false;
        }

        self.snake_just_ate = self.food.update(&self.snake);
        if self.snake_just_ate {
            self.generate_new_food_location();
        }
        return true;
    }

    fn pressed(&mut self, btn: &Button) {
        let last_direction = self.snake.dir.clone();

        self.snake.dir = match btn {
            &Button::Keyboard(Key::Up) if last_direction != Direction::Down => Direction::Up,
            &Button::Keyboard(Key::Down) if last_direction != Direction::Up => Direction::Down,
            &Button::Keyboard(Key::Right) if last_direction != Direction::Left => Direction::Right,
            &Button::Keyboard(Key::Left) if last_direction != Direction::Right => Direction::Left,
            _ => last_direction,
        }
    }

    fn generate_new_food_location(&mut self) {
        let mut r = thread_rng();
        loop {
            let rnd_x = r.gen_range(0..self.cols);
            let rnd_y = r.gen_range(0..self.rows);
            if !self
                .snake
                .body
                .iter()
                .any(|body_part| body_part.x == rnd_x && body_part.y == rnd_y)
            {
                self.food = Food { x: rnd_x, y: rnd_y };
                break;
            }
        }
    }
}

struct Snake {
    body: LinkedList<SnakeBodyPart>,
    dir: Direction,
}

#[derive(Clone)]
struct SnakeBodyPart {
    x: u32,
    y: u32,
}

impl Snake {
    fn render(&self, gl: &mut GlGraphics, args: &RenderArgs, tile_width: u32) {
        let white: [f32; 4] = [1.0, 1.0, 1.0, 1.0];

        let snake_body: Vec<graphics::types::Rectangle> = self
            .body
            .iter()
            .map(|body_part| {
                graphics::rectangle::square(
                    (body_part.x * tile_width) as f64,
                    (body_part.y * tile_width) as f64,
                    tile_width as f64,
                )
            })
            .collect();

        gl.draw(args.viewport(), |ctx, gl| {
            snake_body
                .into_iter()
                .for_each(|square| graphics::rectangle(white, square, ctx.transform, gl));
        });
    }

    fn update(&mut self, just_eaten: bool, rows: u32, cols: u32) -> bool {
        let mut new_head: SnakeBodyPart = (*self.body.front().expect("Snake has no body")).clone();

        if (self.dir == Direction::Up && new_head.y == 0)
            || (self.dir == Direction::Down && new_head.y == rows - 1)
            || (self.dir == Direction::Left && new_head.x == 0)
            || (self.dir == Direction::Right && new_head.x == cols - 1)
        {
            return false;
        }

        match &self.dir {
            Direction::Right => new_head.x += 1,
            Direction::Left => new_head.x -= 1,
            Direction::Up => new_head.y -= 1,
            Direction::Down => new_head.y += 1,
        }

        if !just_eaten {
            self.body.pop_back().expect("Failed to pop_back snake tail");
        }

        if self.collided_with_snake(&new_head) {
            return false;
        }

        self.body.push_front(new_head);

        true
    }

    fn collided_with_snake(&self, head: &SnakeBodyPart) -> bool {
        self.body
            .iter()
            .any(|body_part| body_part.x == head.x && body_part.y == head.y)
    }
}

struct Food {
    x: u32,
    y: u32,
}

impl Food {
    fn render(&self, gl: &mut GlGraphics, args: &RenderArgs, tile_width: u32) {
        let red: [f32; 4] = [1.0, 0.0, 0.0, 1.0];

        let x: u32 = self.x * tile_width;
        let y: u32 = self.y * tile_width;

        let piece_of_food: graphics::types::Rectangle =
            graphics::rectangle::square(x as f64, y as f64, tile_width as f64);

        gl.draw(args.viewport(), |ctx, gl| {
            graphics::rectangle(red, piece_of_food, ctx.transform, gl);
        });
    }

    fn update(&mut self, snake: &Snake) -> bool {
        let head = snake.body.front().expect("Snake has no body");
        if head.x == self.x && head.y == self.y {
            return true;
        }
        return false;
    }
}

fn main() {
    let opengl = OpenGL::V3_2;

    const ROWS: u32 = 20;
    const COLS: u32 = 30;
    const SIZE: u32 = 20;

    let width = COLS * SIZE;
    let height = ROWS * SIZE;

    let mut window: GlutinWindow = WindowSettings::new("snake_using_piston", [width, height])
        .graphics_api(opengl)
        .exit_on_esc(true)
        .build()
        .unwrap();

    let start_x: u32 = COLS / 2;
    let start_y: u32 = ROWS / 2;

    let mut game = Game {
        gl: GlGraphics::new(opengl),
        rows: ROWS,
        cols: COLS,
        tile_width: SIZE,
        food: Food { x: 1, y: 1 },
        snake_just_ate: false,
        score: 0,
        snake: Snake {
            body: LinkedList::from_iter(
                (vec![
                    SnakeBodyPart {
                        x: start_x,
                        y: start_y,
                    },
                    SnakeBodyPart {
                        x: start_x,
                        y: start_y + 1,
                    },
                ])
                .into_iter(),
            ),
            dir: Direction::Right,
        },
    };

    game.generate_new_food_location();

    let mut events = Events::new(EventSettings::new()).ups(8);
    while let Some(e) = events.next(&mut window) {
        if let Some(r) = e.render_args() {
            game.render(&r);
        }

        if let Some(_u) = e.update_args() {
            if !game.update() {
                break;
            }
        }

        if let Some(b) = e.button_args() {
            if b.state == ButtonState::Press {
                game.pressed(&b.button);
            }
        }
    }
    println!("Congratulations your score was {}", game.score);
}
