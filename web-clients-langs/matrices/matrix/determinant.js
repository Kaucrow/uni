module.exports = function (Matrix) {
  Matrix.prototype.det = function() {
    if (!this.isSquare()) {
      throw new Error('Determinant is defined only for square matrices.');
    }

    // Base case for 2x2 matrix
    if (this.rows === 2) {
        return this.data[0][0] * this.data[1][1] - this.data[0][1] * this.data[1][0];
    }

    // Recursive case for larger matrices using Laplace expansion
    let determinant = 0;
    for (let col = 0; col < this.cols; col++) {
        const subMatrix = new Matrix(this.data.slice(1).map(row =>
            row.filter((_, j) => j !== col)
        ));
        const sign = col % 2 === 0 ? 1 : -1; // Alternate signs
        determinant += sign * this.data[0][col] * subMatrix.det();
    }

    return determinant;
  };
};