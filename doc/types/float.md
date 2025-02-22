# Float / std::Float

The float type is used to store a 64-Bit IEEE-754 floating point number.
A value of type float can be created by a float literal e.g. `0.0`, `-1.0` or `42.0`.

**Example**
```js
function main() {
	var a = 42.0;
	var b = 1337.0;
	std::println(a);
	std::println(b);
}
```


## Static Functions

### . new() : Float

Returns `0.0`.

### . is(value : Any) : Bool

Checks whether value is an float.

### . cast(value : Any) : Float

Casts value to an float.