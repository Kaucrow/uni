module.exports = function (Matrix) {
  Matrix.prototype.transpose = function() {
    let result = Array(this.cols).fill().map(() => Array(this.rows).fill(0));

    for (let i = 0; i < this.rows; i++) {
      for (let j = 0; j < this.cols; j++) {
        result[j][i] = this.data[i][j];
      }
    }

    return new Matrix(result);
  };
};