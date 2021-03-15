# ✨Phan✨
## _"A simple, yet a powerful programming language aimed against code repetition"_



Phan is a new programming language developed by Philip Wenkel. It is a simple, yet efficient, programming language written in the high performance language C++. It makes a great antidote for text repetition, whether it is for coding or really anything else. It also makes it much easier to create template files or folder structures. I bet if you are a similar to me, making a new project can take some time to setup. Probably you have some prepared base project structure which you pretty much copy-paste to the new one and just rename everything to fit your current project name. Enough of words, lets look at an example of how to use phan to simplify code repetition in c++.

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

$(0 i 3){
    template <>
    struct Foo <${i}>
    {
        inline static constexpr int i = ${i};  
    };
}
```

That's it!




<!--## Installation

Dillinger requires [Node.js](https://nodejs.org/) v10+ to run.

Install the dependencies and devDependencies and start the server.

```sh
cd dillinger
npm i
node app
```-->


## Development

Want to contribute? Great!






