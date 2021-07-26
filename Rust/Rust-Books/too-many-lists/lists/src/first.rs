// For now we'll avoid generics and keep things simple
// We'll only support storing signed i32's

pub struct List {
    head: Link,
}

struct Node {
    elem: i32,
    next: List,
}

enum Link {
    Empty,
    More(Box<Node>),
}
