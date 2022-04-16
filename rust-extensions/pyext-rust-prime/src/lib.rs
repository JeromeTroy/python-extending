#[macro_use]
extern crate cpython;

use cpython::{Python, PyResult, PyObject, FromPyObject};

fn compute_primes(_py: Python, num_primes_py: &PyObject) -> PyResult<Vec<i64>> {

    let num_primes_result: PyResult<i64> = 
        <i64 as FromPyObject>::extract(_py, num_primes_py);
    let num_primes: i64 = num_primes_result.unwrap();

    let mut primes_found = vec![2];
    let mut number_primes_found = 1;
    
    let mut prime_candidate = 3;
    let mut is_prime = true;

    while number_primes_found < num_primes {
        for prime in primes_found.iter() {
            if prime_candidate % prime == 0 {
                is_prime = false;
                break;
            }
        }
        if is_prime {
            primes_found.push(prime_candidate);
            number_primes_found += 1;
        }

        is_prime = true;
        prime_candidate += 1;
    }

    return Ok(primes_found);
}

py_module_initializer!(librustprime, initlibrustprime, 
                       PyInit_rustprime, 
                       | py, m | {
                           r#try!(m.add(py, "__doc__", 
                                      "Prime finder in Rust"));
                           r#try!(m.add(py, "compute_primes", 
                                      py_fn!(py, 
                                        compute_primes(num_primes: &PyObject))));
                           Ok( () )
                       });
