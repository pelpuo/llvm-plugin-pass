; ModuleID = 'newdemo.bc'
source_filename = "demo"

define i32 @main() {
entry:
  %x = alloca i32, align 4
  store i32 42, ptr %x, align 4
  br label %loop

loop_body:                                        ; preds = %loop
  %z = alloca i32, align 4
  store i32 2, ptr %z, align 4
  br label %loop

loop:                                             ; preds = %loop_body, %entry
  %y = alloca i32, align 4
  store i32 1, ptr %y, align 4
  %val = load i32, ptr %x, align 4
  %cond = icmp eq i32 %val, 42
  br i1 %cond, label %exit, label %loop_body

exit:                                             ; preds = %loop
  %result = load i32, ptr %x, align 4
  ret i32 %result
}
