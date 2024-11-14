const Matrix = require('./matrix/Matrix')
const readline = require('node:readline');

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
});

function getRowColsInput(query) {
  return new Promise(resolve =>
    rl.question(query, input => {
      const num = Number(input);
      if (!isNaN(num) && Number.isInteger(num) && num > 0) {
        resolve(num);
      } else {
        console.log('Please enter a positive integer number.');
        resolve(getRowColsInput(query));
      }
    })
  );
}

function getNumberInput(query) {
  return new Promise(resolve =>
    rl.question(query, input => {
      const num = Number(input);
      if (!isNaN(num)) {
        resolve(num);
      } else {
        console.log('Please enter a number.');
        resolve(getNumberInput(query));
      }
    })
  );
}

function getInput(query) {
  return new Promise(resolve => rl.question(query, resolve));
}

(async function main() {
  while (true) {
    console.clear();

    console.log('*** Getting matrix A properties ***');
    const matrixARows = await getRowColsInput('Number of rows: ');
    const matrixACols = await getRowColsInput('Number of columns: ');
    let matrixAData = Array(matrixARows).fill().map(() => Array(matrixACols));
    for (let i = 0; i < matrixARows; i++) {
      for (let j = 0; j < matrixACols; j++) {
        matrixAData[i][j] = await getNumberInput(`Number at ${i}, ${j}: `);
      }
    }

    console.log('\n*** Getting matrix B properties ***');
    const matrixBRows = await getRowColsInput('Number of rows: ');
    const matrixBCols = await getRowColsInput('Number of columns: ');
    let matrixBData = Array(matrixBRows).fill().map(() => Array(matrixBCols));
    for (let i = 0; i < matrixBRows; i++) {
      for (let j = 0; j < matrixBCols; j++) {
        matrixBData[i][j] = await getNumberInput(`Number at ${i}, ${j}: `);
      }
    }
    
    console.log('\n*** Getting other test values ***');
    const scalar = await getNumberInput('Scalar to multiply matrix A by: ');

    const matrixA = new Matrix(matrixAData);
    const matrixB = new Matrix(matrixBData);

    console.log('\nMatrix A: ');
    console.log(matrixA.toString() + '\n');
    console.log('Matrix B: ');
    console.log(matrixB.toString() + '\n');

    console.log('*** Displaying test operations ***\n');

    let transpose = matrixA.transpose();
    console.log('-> Matrix A transpose: ');
    console.log(transpose.toString());
    
    let dotProd = matrixA.dotProduct(scalar);
    console.log(`-> Matrix A * ${scalar}: `);
    console.log(dotProd.toString());

    // Check if matrix A and matrix B can be added together
    if (matrixA.rows === matrixB.rows && matrixA.cols === matrixB.cols) {
      let sum = matrixA.sum(matrixB);
      let sub = matrixA.sub(matrixB);

      console.log('-> Matrix A + Matrix B: ');
      console.log(sum.toString());
      
      console.log('-> Matrix A - Matrix B: ');
      console.log(sub.toString());
    }

    // Check if matrix A can be multiplied by matrix B
    if (matrixA.cols === matrixB.rows) {
      let mult = matrixA.mult(matrixB);
      console.log('-> Matrix A * Matrix B: ');
      console.log(mult.toString());
    }

    // Check if matrix A is square
    if (matrixA.rows === matrixA.cols) {
      let identity = matrixA.identity();
      console.log('-> Matrix A identity: ');
      console.log(identity.toString());
      
      let determinant = matrixA.det();
      console.log(`-> Matrix A determinant: ${determinant}`);

      // Check if matrix A has an inverse
      if (determinant !== 0) {
        let inverse = matrixA.inverse();
        console.log('-> Matrix A inverse: ');
        console.log(inverse.toString());
      }
    }

    const input = await getInput('Would you like to process another matrix? (y/n): ');

    if (input.toLowerCase() !== 'y') {
      break;
    }
  }
  rl.close()
})();