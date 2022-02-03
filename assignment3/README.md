## Find language from target text file

How to compile findlang.cpp
```bash
g++ findlang.cpp -o findlang
```

How to run findlang.cpp <br>
Search using cache
```bash
./findlang cache <target_text_file> <context_size> <smoothing_percentage>
```

Search using finite context model
```bash
./findlang langs <target_text_file> <context_size> <smoothing_percentage>
```
*Note: The target text files to test this class are stored in directory "examples"


## Add new language to cache
How to compile newlang.cpp
```bash
g++ newlang.cpp -o newlang
```

How to run newlang.cpp
run (search using cache)
```bash
./newlang <new_language_file>
```

## Test FCM class
How to compile fcm_demo.cpp
```bash
g++ fcm_demo.cpp -o fcm_demo
```

How to run newlang.cpp
run (search using cache)
```bash
./fcm_demo 
```
