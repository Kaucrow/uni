module.exports = function (Matrix) {
  Matrix.prototype.sub = function (otherMatrix) {
    if (this.rows !== otherMatrix.rows || this.cols !== otherMatrix.cols) {
      throw new Error('Matrices must have the same dimensions to be substracted.');
    }

    const result = this.data.map((row, i) =>
      row.map((value, j) => value - otherMatrix.data[i][j])
    );

    return new Matrix(result);
  };
};