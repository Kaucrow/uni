module.exports = function (Matrix) {
  Matrix.prototype.identity = function() {
    if (!this.isSquare()) {
      throw new Error('The matrix must be square to have an identity matrix.');
    }

    let data = this.data.map((row, i) =>
      row.map((_, j) => (
        i == j ? 1 : 0
      ))
    );

    return new Matrix(data);
  };
};