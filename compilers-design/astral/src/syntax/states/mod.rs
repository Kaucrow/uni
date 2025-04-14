pub mod normal;
pub mod expressions;

pub use super::*;

impl PDA {
    pub fn build(&mut self) {
        self.add_normal_states();
        self.add_expression_states();
    }
}