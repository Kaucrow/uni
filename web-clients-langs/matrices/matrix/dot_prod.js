module.exports = function (Matrix) {
  Matrix.prototype.dotProduct = function(scalar) {
    const result = this.data.map((row, _) =>
      row.map((val, _) => val * scalar)
    );

    return new Matrix(result);
  };
};