module.exports = function (Matrix) {
  Matrix.prototype.inverse = function() {
    if (!this.isSquare()) {
      throw new Error('The matrix must be square to have an inverse matrix.');
    }

    if (this.det() === 0) {
      throw new Error('The matrix must be have a non-zero determinant to have an inverse matrix.');
    }

    let matrix = this;
    let inverse = this.identity();

    // Perform row swaps if any pivot is 0
    let swaps = [];   // Stores the idx of the swapped rows 
    for (let { val: pivot, idx: pivotIdx } of matrix.diagonal()) {
      if (pivot === 0) {
        for (let row = 0; row < matrix.rows; row++) {
          if (matrix.data[row][pivotIdx] !== 0 && !swaps.includes(row)) {
            for (let col = 0; col < matrix.cols; col++) {
              // Swap the rows of the matrix
              {
                let temp = matrix.data[pivotIdx][col];
                matrix.data[pivotIdx][col] = matrix.data[row][col];
                matrix.data[row][col] = temp;
              }

              // Do the same with the inverse
              {
                let temp = inverse.data[pivotIdx][col];
                inverse.data[pivotIdx][col] = inverse.data[row][col];
                inverse.data[row][col] = temp;
              }
            }
            // Add the current pivot row to the swaps array
            swaps.push(pivotIdx);
            break;
          }
        }
      }
    }

    for (let { val: pivot, idx: pivotIdx } of matrix.diagonal()) {
        for (let row = 0; row < matrix.rows; row++) {
          if (pivotIdx === row) {
            continue;
          }

          let multip = -1 * matrix.data[row][pivotIdx] / pivot;

          for (let col = 0; col < matrix.cols; col++) {
            // For every value in the multiplier's row, multiply it by the multiplier
            // and add each result to each value of the current row
            {
              let currRowVal = matrix.data[row][col];
              let pivotRowVal = matrix.data[pivotIdx][col];
              currRowVal += pivotRowVal * multip;
              matrix.data[row][col] = currRowVal;
            }

            // Do the same with the inverse matrix
            {
              let currRowVal = inverse.data[row][col];
              let pivotRowVal = inverse.data[pivotIdx][col];
              currRowVal += pivotRowVal * multip;
              inverse.data[row][col] = currRowVal;
            }
          }
        }
    };

    for (let { val: pivot, idx: pivotIdx } of matrix.diagonal()) {
      for (let col = 0; col < matrix.cols; col++) {
        matrix.data[pivotIdx][col] = matrix.data[pivotIdx][col] / pivot;
        inverse.data[pivotIdx][col] = inverse.data[pivotIdx][col] / pivot;
      }
    }

    return inverse;
  };
};