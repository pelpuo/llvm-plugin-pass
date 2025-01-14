; ModuleID = 'newdemo.bc'
source_filename = "example"

define i32 @main() {
new_block:
  %result = alloca i32, align 4
  store i32 174, ptr %result, align 4
  ret i32 174
}
