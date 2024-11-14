module.exports = function (Matrix) {
  Matrix.prototype.mult = function (otherMatrix) {
    if (this.cols !== otherMatrix.rows) {
      throw new Error('The number of columns in the first matrix must be equal to the number of rows in the second matrix for multiplication to be possible.');
    }

    const result = this.data.map((row, _) =>
      // For each row in matrix A, create a new row
      // with the number of columns of matrix B
      new Array(otherMatrix.cols).fill(0).map((_, j) =>
        // For each value in the current row of matrix A,
        // perform the multiplication with the corresponding value of
        // matrix B, and then add the results of the multiplications
        row.reduce((sum, value, k) => sum + value * otherMatrix.data[k][j], 0)
      )
    );

    return new Matrix(result);
  };
};