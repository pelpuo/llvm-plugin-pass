; ModuleID = 'newdemo.bc'
source_filename = "demo"

define i32 @main() {
entry:
  %x = alloca i32, align 4
  store i32 42, ptr %x, align 4
  br label %loop

additional_block_5:                               ; No predecessors!
  %i = load i32, ptr %x, align 4
  %j = xor i32 %i, 15
  store i32 %j, ptr %x, align 4
  br label %loop

additional_block_4:                               ; No predecessors!
  %g = load i32, ptr %x, align 4
  %h = or i32 %g, 8
  store i32 %h, ptr %x, align 4
  br label %loop

additional_block_3:                               ; No predecessors!
  %e = load i32, ptr %x, align 4
  %f = sdiv i32 %e, 3
  store i32 %f, ptr %x, align 4
  br label %loop

additional_block_2:                               ; No predecessors!
  %c = load i32, ptr %x, align 4
  %d = sub i32 %c, 5
  store i32 %d, ptr %x, align 4
  br label %loop

additional_block_1:                               ; No predecessors!
  %a = load i32, ptr %x, align 4
  %b = mul i32 %a, 2
  store i32 %b, ptr %x, align 4
  br label %loop

loop_body:                                        ; No predecessors!
  %z = alloca i32, align 4
  store i32 2, ptr %z, align 4
  br label %loop

loop:                                             ; preds = %additional_block_5, %additional_block_4, %additional_block_3, %additional_block_2, %additional_block_1, %check_value, %loop_body, %entry
  %y = alloca i32, align 4
  store i32 1, ptr %y, align 4
  %val = load i32, ptr %x, align 4
  %cond = icmp eq i32 %val, 42
  br i1 %cond, label %exit, label %check_value

check_value:                                      ; preds = %loop
  %temp = load i32, ptr %x, align 4
  %new_val = add i32 %temp, 1
  store i32 %new_val, ptr %x, align 4
  br label %loop

exit:                                             ; preds = %loop
  %result = load i32, ptr %x, align 4
  ret i32 %result
}