use numpy::PyArray1;
use pyo3::{
    types::{PyAnyMethods, PyDict, PyList},
    Python,
};

fn main() {
    pyo3::prepare_freethreaded_python();
    let data: Vec<_> = (0..(10 * 1024 * 1024 / 8)).into_iter().collect();
    for _ in 0..10 {
        Python::with_gil(|py| {
            let arr = PyArray1::from_slice_bound(py, &data);
            // let arr = PyList::new_bound(py, &data);
            let vars = PyDict::new_bound(py);
            let locals = PyDict::new_bound(py);
            vars.set_item("arr", arr).unwrap();
            vars.set_item(
                "__builtins__",
                py.import_bound("__main__")
                    .unwrap()
                    .getattr("__builtins__")
                    .unwrap(),
            )
            .unwrap();
            py.run_bound(
                r#"  
import psutil
mem = psutil.Process().memory_info().rss / 1024 / 1024
print(mem)

def foo(arr):
    return arr * 2

arr2 = foo(arr)
                "#,
                Some(&vars),
                // Some(&locals),
                None,
            )
            .unwrap();
        });
    }
}
