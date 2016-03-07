# jo

![jo logo](jo-logo.png)

This is `jo`, a small utility to create JSON objects

```bash
$ jo -p name=jo n=17 parser@0
{
    "name": "jo",
    "n": 17,
    "parser": false
}
```

or arrays

```bash
$ seq 1 10 | jo -a
[1,2,3,4,5,6,7,8,9,10]
```

To build it, type `make`. It has a [manual](jo.md), and you can read [why I wrote jo](http://jpmens.net/2016/03/05/a-shell-command-to-create-json-jo/).


