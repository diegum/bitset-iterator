# bitset-iterator

An iterator for C++ STL `std::bitset`.


## What's about

Among the many uses that the STL template type `std::bitset` has, a possible one is a set of integers.

For example, consider an instance of `std::bitset<8>` like `0b01001011`.

Such instance can be seen as the set of integers `{ 0, 1, 3, 6 }`. These are the indices of all set bits.

Yet, why not, it could be also seen as the set of integers `{ 2, 4, 5, 7 }`. These are the indices of all unset bits.

As a generic bitset type, however, this STL template isn't necessarily mean to model sets of integers. An `std::bitset` is primarily a set of bits (e.g., flags on/off, etc.)

Using `std::bitset` to model a set of integers could be a matter of convenience given its compact runtime footprint. Also, the ability to load it from, unload it to an unsigned number.

Unfortunately, for those scenarios where `std::bitset` is a smart choice to represent sets of integers, the type provides no iterators. There's no convenience way to retrieve just the bits that are on (or, conversely, the ones off) other than just visit all bits and test their statuses.

This header-only template library closes such gap.


## Usage example

Given a bitset, imagine that you just need to print indices for bits that are on.

```cpp
#include "bitset/bitset_iter.h"

int main()
{
  std::bitset<10> my_set_of_integers{ 0b10110010 };

  std::cout << "Indices \"ON\" are";
  for (const auto& index : bitset::indices_on(my_set_of_integers)) {
    std::cout << ' ' << index;
  }
  std::cout << std::endl;
}
```

This code outputs

    Indices "ON" are 1 4 5 7

You could have still got the same without this library:

```cpp
int main()
{
  std::bitset<10> my_set_of_integers{ 0b10110010 };

  std::cout << "Indices \"ON\" are";
  for (auto index = 0; index < 10; ++index) {
    if (my_set_of_integers[index]) {
      std::cout << ' ' << index;
    }
  }
  std::cout << std::endl;
}
```

No big deal! It's just that when you find yourself doing this same _iterate-by-testing-and-if-true-do-something_ ceremony over and over, you start wondering whether that could be encapsulated in a reusable iterator. Such iterator skips indices you aren't interested, only giving you the ones you care.

This library primarily provides two main functions: `bitset::indices_on(std::bitset<N>)` and `bitset::indices_off(std::bitset<N>)`.

`indices_on` returns an iterator over all bits set to on. `indices_off` returns an iterator over all bits set to off.

Notice that, given an `std::bitset some_bitset`, iterators `indices_on(some_bitset)` and `indices_off(~some_bitset)` produce the same indices when dereferenced.

Ideally, you shouldn't deal with the iterator yourself. Just call either of these functions in a _range-based for loop_. This kind of loop uses the iterator internally so you'll be OK.

[![try it online](https://camo.githubusercontent.com/e59a09f0614f475d5ebcb0e8d78c595eea567622/68747470733a2f2f696d672e736869656c64732e696f2f62616467652f74727925323069742d6f6e6c696e652d627269676874677265656e2e737667 "try it online")](https://wandbox.org/permlink/rSdCUV0r2hBgxmPf)


## How to install

This is a header-only template library. Just get a copy of [`src/bitset_iter.h`](https://github.com/diegum/bitset-iterator/blob/master/src/bitset_iter.h) and place it handy in your project.

I suggest you to create a bitset directory (e.g., `your_project/third_party/bitset`). Then, add that route to the `--include-directory=<path>` flag (`-I <path>` for short).



## Frequently Asked Question (FAQ)

### What's the big deal with this library? What am I missing if I don't apply it?
You won't miss so much. But if you needed to iterate over all bits on (or, conversely, off) of a bitset, if at all, you'd be on your own. That's what it happened to me so frequently that I decided to create this library.

### Is it any faster than manual iteration through the bitset?
It's about the same. Under the hood, it visits all indices, discarding the ones that don't matter. If you don't have to do that check in your own code, it's because it's happening under the hood. Not that it's not happening anymore.
Yet, your code becomes concise and also elegant: stack up against the _range-based for loop_, the _traditional for loop_ looks... prehistoric rather than traditional.

### How does impact to the footprint?
Good thing about C++ Templates and Function Templates is that they only affect the footprint when they are used.

The iterator type is really lightweight and optimized. It keeps a reference to the original bitset unless the original bitset is a volatile _R-value_. Before an _R-value_, the iterator keeps a copy of the original bitset.

### What if the bits get modified while iterating?
As told above, the iterator keeps a reference to the bitset if this is an _L-value_. As a consequence of this, any change to the original bitset will be taken into account by the iterator.

Imagine, for example, that the process has two threads. In one thread, we start iterating over the bits set to on for a bitset whose 10th bit is on.

While we are iterating by its 6th bit, the other thread turns the 10th bit off. If it remains like that when the iterator visits it, it will skip that bit.

Conversely, turning on a bit that was off at the time we started iterating, will make the iterator to stop at it when visited.

It must be said that STL `std::bitset` ain't thread-safe. If a bit is getting modified while the iterator is checking it, that will result on a race condition.

### Why not `std::vector<bool>`, which is also optimized like `std::bitset<N>` but does even provide iterators?
Yes, it provides iterators but these loop through all elements. You don't get the kind of _filtering views_ just over those that are on or conversely off.

But that ain't all.

All STL manuals and top C++ sites recommend using `std::bitset<N>` when you know the value of `N` beforehand. In that regard, `std::vector<bool>` is optimized but doesn't guarantee that its internal data will be contiguous, as it dynamically grows as you `push_back` elements.

There's more. I left this reason for the end.

`std::vector<bool>` can't be loaded from or unload to a, namely, `unsigned long` or `unsigned long long`. It's optimized at runtime, exclusively. `std::bitset<N>` is optimized for both runtime and also persistence as it can be constructed from a number, exposes functions like `to_ulong()` or `to_ullong()`. If it just had a convenient way to iterate through relevant data would be perfect. Well, here's one.

### Why is `std::size_t` the type of the dereferenced element?
For consistency with `std::bitset`: its functions `operator[](std::size_t)`, `set(std::size_t, bool)` or `test(std::size_t)` all receive an `std::size_t` as index or bit position.

This means that there is no way to access a bit in an `std::bitset` beyond the maximum number representable with an `std::size_t`.

Therefore, dereferencing the iterator can't lead to values outside the scope of `std::size_t`.

### What is the maximum integer that an `std::bitset`, seen as a set of integers, can hold?
The rule of thumb here is `(sizeof(std::size_t) << 3) - 1`. For example, in 64-bit architectures where `std::size_t` is 8 bytes, such maximum would be `63`.

This is because each byte stores 8 indices (so we must multiple the result of `sizeof` by `8`, or just shift it `3` positions to the left). We must subtract `1` because the base is `0`.

The maximum index would be `31` in 32-bit architectures where `std::size_t` is 4 bytes.

### Does it leverage machine instructions like _find first set (ffs)_, _count trailing zeroes (ctz)_ or _bit scan forward (bsf)_, to mention some? These are available in most of machines. Does it leverage these at all?
No.

---

# For Developers who want to fork this project

This C++ library is cross-platform and comes with a terminal sample app that shows how to include it.

## Building requirements

Make sure you've installed [Buck](https://buckbuild.com/setup/install.html).

### Building and running

You can build the demo applications from the command line! For all the following, just make sure that you are at the project root.

    $ buck run terminal

### Testing

This library aggregates a Catch/2 test framework. To build and run tests:

    $ buck run test

### Style checking

You'll need [Cppcheck](http://cppcheck.sourceforge.net) in your machine. Make sure that no code change ain't committed without checking C++ coding style.

    $ buck run lint

### Building the documentation

You'll need [Doxygen](http://www.doxygen.nl) in your machine. Make sure that any revision to the code is backed by Doxygen-savvy comments.

    $ buck run doc
