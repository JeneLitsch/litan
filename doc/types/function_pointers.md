# Function / std::Function

Function pointers are callable that can be saved and passed around to invoke later. They can be used to implement higher order functions.

**Example**
```js
function main() {
	var str = "Hello World";
	var fx_ref    = &std::println(_);
	var fx_lambda = lambda(x) => std::println(x);
	fx_ref(str);
	fx_lambda(str);
}
```

## Static Functions

### . is(value : Any) : Bool

Checks whether value is a function.

### . cast(value : Any) : Function

Returns value if value is a function. Otherwise null is returned.


## Member Functions

### -> arity(): Int

Return the number of expected non-variadic parameters;

### -> is_variadic(): Bool

Returns whether the function is variadic.

### -> call(param: Array|Tuple): Any

Calls the function.


## Operators

### Call Operator ()

Calls the function.



## From lambdas

Any lambda yields an funtion pointer.

## From functions

A function pointer to a regular function can be create by prefixing the function with `&` and replacing all parameters with `_`(underscores).