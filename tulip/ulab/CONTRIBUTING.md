Contributions of any kind are always welcome. 

# Contributing to the code base

If you feel like adding to the code, you can simply issue a pull request. If you do so, please, try to adhere to `micropython`'s [coding conventions](https://github.com/micropython/micropython/blob/master/CODECONVENTIONS.md#c-code-conventions).

# Documentation

However, you can also contribute to the documentation (preferably via the [jupyter notebooks](https://github.com/v923z/micropython-ulab/tree/master/docs). 

## Testing

If you decide to lend a hand with testing, here are the steps:

1. Write a test script that checks a particular function, or a set of related functions!
1. Drop this script in one of the folders in [ulab tests](https://github.com/v923z/micropython-ulab/tree/master/tests)!
1. Run the [./build.sh](https://github.com/v923z/micropython-ulab/blob/master/build.sh) script in the root directory of `ulab`! This will clone the latest `micropython`, compile the firmware for `unix`, execute all scripts in the `ulab/tests`, and compare the results to those in the expected results files, which are also in `ulab/tests`, and have an extension `.exp`. In case you have a new snippet, i.e., you have no expected results file, or if the results differ from those in the expected file, a new expected file will be generated in the root directory. You should inspect the contents of this file, and if they are satisfactory, then the file can be moved to the `ulab/tests` folder, alongside your snippet. 
