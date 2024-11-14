class Matrix {
  constructor(data) {
    if (!Array.isArray(data) || !data.every(row => Array.isArray(row))) {
      throw new Error('Matrix data must be a 2D array.');
    }

    const numColumns = data[0].length;
    if (!data.every(row => row.length === numColumns)) {
      throw new Error('All rows in the matrix must have the same number of columns.');
    }

    this.data = data;
    this.rows = data.length;
    this.cols = numColumns;
  }

  toString() {
    return this.data.map(row => row.join(' ')).join('\n');
  }

  isSquare() {
    return this.rows === this.cols;
  }

  *diagonal() {
    const size = Math.min(this.rows, this.cols);
    for (let i = 0; i < size; i++) {
      yield { val: this.data[i][i], idx: i };
    }
  }
}

require('./sum.js')(Matrix);
require('./substract.js')(Matrix);
require('./multiply.js')(Matrix);
require('./identity.js')(Matrix);
require('./determinant.js')(Matrix);
require('./inverse.js')(Matrix);
require('./dot_prod.js')(Matrix);
require('./transpose.js')(Matrix);

module.exports = Matrix;