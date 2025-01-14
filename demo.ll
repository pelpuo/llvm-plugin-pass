; ModuleID = 'demo'
source_filename = "demo"

define i32 @main() {
entry:
  %x = alloca i32
  store i32 42, i32* %x
  br label %loop

loop:
  %y = alloca i32
  store i32 1, i32* %y
  %val = load i32, i32* %x
  %cond = icmp eq i32 %val, 42
  br i1 %cond, label %exit, label %loop_body

loop_body:
  %z = alloca i32
  store i32 2, i32* %z
  br label %loop

exit:
  %result = load i32, i32* %x
  ret i32 %result
}
