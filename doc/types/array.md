# Array / std::Array

The array type is used store a dynamic array of elements of any type.

## Static Functions

### . new() : Array

Creates a new and empty array.

### . is(value : Any) : Bool

Checks whether value is an array

### . cast(value : Any) : Array

Returns value if value is an array. Otherwise null is returned.

## Member Functions

### -> size(): Int

Return the size the array as an int;

### -> is_empty(): Bool

Returns whether the array is empty (size == 0)

### -> push(value : Any): Null

Append a single value to the end of the array

### -> pop(): Any

Remove a single value from the end the array.  
The removed value is returned.  
Throws an exception if the array is empty.

### -> front(): Any

Return the value at index 0.  
Throws an exception if the array is empty.

### -> back(): Any

Return the value at the end of the array.  
Throws an exception if the array is empty.

### -> peek(): Any

Return the value at the end of the array.  
Throws an exception if the array is empty.

### -> at(index : Int): Any

Returns the value at the given index.  
Throws an exception if the array is empty or the index is out of range.


### -> insert(index : Int, value): Null

Inserts a value at the given index.  
Throws an exception if the array is empty or the index is out of range.


### -> erase(index : Int): Any

Erases the value at the given index.
The removed value is returned.  
Throws an exception if the array is empty or the index is out of range.


### -> values(): Array

Returns a shallow copy.


### -> filter(f : Function): Array

Creates a shallow copy containing the elements for which `f` returns a truthy result.


### -> transform(f : Function): Array

Calls `f` for all elements and create an array containing the result.


### -> reduce(f : Function): Any

Sums up all elements using `f`.
Returns `null` if the array is empty.


### -> any(f : Function): Bool

Tests whether `f` returns a truthy value for any element in the array.


### -> all(f : Function): Bool

Tests whether `f` returns a truthy value for all elements in the array.


### -> none(f : Function): Bool

Tests whether `f` returns a truthy value for no element in the array.


### -> has(value : Any): Bool

Tests whether the array contains the given value.


### -> slice(start : Int, end : Int): Array

Returns a copied slice of the array.
If the size is negative an out of range exception is thrown.
If the start or end of the slice is out of range an exception is thrown.  


### -> prefix(n : Int): Array

Return a slice containing the first n elements in order.
Same as `arr->slice(0, size)`.


### -> suffix(n : Int): Array

Return a slice containing the last n elements in order.
Same as `arr->slice(arr->size() - size, arr->size())`.


### -> reversed(start : Int, end : Int): Array

Returns a copy of the array in reversed order.


### -> zipped(other : Array): Array(Tuple)

Returns an array of tuples. Each tuple contains the elements that are at the same index.
If one array is larger than the other, extraneus elements will be ignored.

```js
var a = [1,2,3];
var b = [4,5,6,7];
var c = a->zipped(b); // c == [(1,4), (2,5), (3,6)]
```




## Operators

### [index : int]

Returns the value at the given index. 
Throws an exception if the array is empty or the index is out of range.

### array + array

Concatenate two arrays.

### array * int

Repeat an array.

### int * array

Repeat an array.



## Literal

A new array can be create by writing a comma separated list between `[` and `]` e.g. `[1, 2, 3,]` or `[42, 42]`. 
A single trailing comma is allowed.

Example:
```js
function main() {
	var array = [1, 2, 3,];
	std::println(array);
}
```