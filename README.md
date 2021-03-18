# ✨Phan✨
## _"A simple, yet a powerful programming language aimed against code repetition"_



Phan is a new programming language developed by Philip Wenkel. It is a simple, yet efficient, programming language written in the high performance language C++. It makes a great antidote for text repetition, whether it is for coding or really anything else. It also makes it much easier to create template files or folder structures for your projects, which drastically improves your production time and prevents you from making simple errors. I bet that, if you are like most people, creating a new project can really take time and effort. Probably you have some prepared base project structure which you pretty much copy-paste to the new one and just rename everything to fit your current project name. Enough of words, lets look at an example of how to use phan to simplify code repetition in c++. Then, we will look at how to use phan as a tool when writing a simple document. Last but not least, we will see how it can also be used with files and folders.

```c++
template <int>
struct Foo;

template <>
struct Foo <0>
{
    inline static constexpr int i = 0;  
};

template <>
struct Foo <1>
{
    inline static constexpr int i = 1;  
};

template <>
struct Foo <2>
{
    inline static constexpr int i = 2;  
};
```

As you can see, every template specialization of Foo is pretty much the same, except for two places, where only a number changes. Lets use phan to help us out with this boring and cumbersome code repetition.

```c++
template <int>
struct Foo;

@(type){inline static constexpr int}

$(0 i 3){
    template <>
    struct Foo <${i}>
    {
        ${type} i = ${i};  
    };
}
```

That's it!

Lets write a cv for our new job application!

```c++
@(first name){Philip}
@(last name){Wenkel}
@(name){${first name} ${last name}}
@(company name){Google}

My name is ${name} and I am interested in $(job){coding} at your company ${company name}.
On my spare time, i love ${job}! #{elaborate on this one...}

Yours sincerely, ${name}
```

This will result in the following output:

```c++
My name is Philip Wenkel and I am interested in coding at your company Google.
On my spare time, i love coding!

Yours sincerely, Philip Wenkel
```




## Installation

Phan requires [CMake](https://cmake.org) 3.19.4 to run.



```sh
cd phan
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release  
cmake --install . --config Release  
```

## Usage
```sh
phan input_file.txt output_file.txt
```


## Development

Want to contribute? Great!






