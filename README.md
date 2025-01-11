### Building 

```
cmake –G Ninja ../countirpass

ninja
```

### Running 
```
opt --load-pass-plugin=lib/CountIR.so --passes="countir" --disable-output --stats demo.ll
```