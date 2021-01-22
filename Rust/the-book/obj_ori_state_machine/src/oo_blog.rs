/** TODO:
 *  - duplication includes the similar implementations of the
 *  request_review and approve methods on Post. Both methods
 *  delegate to the implementation of the same method on the
 *  value in the state field of Option and set the new value
 *  of the state field to the result. If we had a lot of
 *  methods on Post that followed this pattern, we might
 *  consider defining a macro to eliminate the repetition
 */

pub struct Post {
    // Need to wrap in Option<T> b/c w/o it any call to
    // state.fcn() will try and 'move' the boxed::Box<>
    // heap memory which can't happen without the Copy
    // trait implemented which it won't be. So we wrap it
    // in an Option<T> and then we can fill the
    // vacated/'move'd heap memory with a None so the
    // compiler won't complain.

    // My inution for the need to wrap the trait obj State
    // in a Box<T> pointer is the same reason you can't
    // make a stack reference to an interface (pure virtual
    // class) in c++. That's why even the methods in the
    // trait obj definition return Box<dyn State>'s.
    state: Option<Box<dyn State>>,
    content: String,
    approval_stamps: i32,
}

impl Post {
    pub fn new() -> Post {
        Post {
            state: Some(Box::new(Draft {})),
            content: String::new(),
            approval_stamps: 0,
        }
    }
    pub fn add_text(&mut self, text: &str) {
        self.content
            .push_str(self.state.as_ref().unwrap().add_text(text));
    }
    pub fn content(&self) -> &str {
        self.state.as_ref().unwrap().content(self)
    }
    pub fn request_review(&mut self) {
        if let Some(s) = self.state.take() {
            self.state = Some(s.request_review());
        }
    }
    pub fn approve(&mut self) {
        self.approval_stamps += 1;
        if let Some(s) = self.state.take() {
            self.state = Some(s.approve(&self));
        }
    }
    pub fn reject(&mut self) {
        self.approval_stamps = 0;
        if let Some(s) = self.state.take() {
            self.state = Some(s.reject());
        }
    }
}

trait State {
    fn request_review(self: Box<Self>) -> Box<dyn State>;
    fn approve(self: Box<Self>, post: &Post) -> Box<dyn State>;
    fn content<'a>(&self, _post: &'a Post) -> &'a str {
        ""
    }
    fn reject(self: Box<Self>) -> Box<dyn State>;
    fn add_text<'a>(&self, _text: &'a str) -> &'a str {
        ""
    }
}

struct Draft {}

impl State for Draft {
    fn request_review(self: Box<Self>) -> Box<dyn State> {
        Box::new(PendingReview {})
    }
    fn approve(self: Box<Self>, _post: &Post) -> Box<dyn State> {
        Box::new(PendingReview {})
    }
    fn reject(self: Box<Self>) -> Box<dyn State> {
        self
    }
    fn add_text<'a>(&self, text: &'a str) -> &'a str {
        text
    }
}

struct PendingReview {}

impl State for PendingReview {
    fn request_review(self: Box<Self>) -> Box<dyn State> {
        self
    }
    fn approve(self: Box<Self>, post: &Post) -> Box<dyn State> {
        if post.approval_stamps >= 2 {
            return Box::new(Published {});
        }
        Box::new(PendingReview {})
    }
    fn reject(self: Box<Self>) -> Box<dyn State> {
        Box::new(Draft {})
    }
}

struct Published {}

impl State for Published {
    fn request_review(self: Box<Self>) -> Box<dyn State> {
        self
    }
    fn approve(self: Box<Self>, _post: &Post) -> Box<dyn State> {
        self
    }
    fn content<'a>(&self, post: &'a Post) -> &'a str {
        &post.content
    }
    fn reject(self: Box<Self>) -> Box<dyn State> {
        self
    }
}

#[cfg(test)]
mod test {
    use crate::oo_blog::Post;

    #[test]
    fn returns_empty_content() {
        let post = Post::new();
        assert_eq!("", post.content());
    }
}
