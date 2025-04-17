; ModuleID = 'file.c'
source_filename = "file.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %x = alloca i32, align 4
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  store volatile i32 4, ptr %x, align 4
  store i32 0, ptr %result, align 4
  store i32 1, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %cmp = icmp sle i32 %0, 5
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load i32, ptr %i, align 4
  %rem = srem i32 %1, 2
  %cmp1 = icmp eq i32 %rem, 0
  br i1 %cmp1, label %if.then, label %if.else

if.then:                                          ; preds = %for.body
  %2 = load i32, ptr %i, align 4
  %3 = load i32, ptr %result, align 4
  %add = add nsw i32 %3, %2
  store i32 %add, ptr %result, align 4
  br label %if.end

if.else:                                          ; preds = %for.body
  %4 = load i32, ptr %i, align 4
  %5 = load i32, ptr %result, align 4
  %sub = sub nsw i32 %5, %4
  store i32 %sub, ptr %result, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %6 = load i32, ptr %i, align 4
  %inc = add nsw i32 %6, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !4

for.end:                                          ; preds = %for.cond
  %7 = load volatile i32, ptr %x, align 4
  switch i32 %7, label %sw.default [
    i32 2, label %sw.bb
    i32 4, label %sw.bb2
  ]

sw.bb:                                            ; preds = %for.end
  %8 = load i32, ptr %result, align 4
  %mul = mul nsw i32 %8, 2
  store i32 %mul, ptr %result, align 4
  br label %sw.epilog

sw.bb2:                                           ; preds = %for.end
  %9 = load i32, ptr %result, align 4
  %rem3 = srem i32 %9, 3
  %cmp4 = icmp eq i32 %rem3, 0
  br i1 %cmp4, label %if.then5, label %if.else7

if.then5:                                         ; preds = %sw.bb2
  %10 = load i32, ptr %result, align 4
  %add6 = add nsw i32 %10, 10
  store i32 %add6, ptr %result, align 4
  br label %if.end9

if.else7:                                         ; preds = %sw.bb2
  %11 = load i32, ptr %result, align 4
  %sub8 = sub nsw i32 %11, 5
  store i32 %sub8, ptr %result, align 4
  br label %if.end9

if.end9:                                          ; preds = %if.else7, %if.then5
  br label %sw.epilog

sw.default:                                       ; preds = %for.end
  store i32 -1, ptr %result, align 4
  br label %sw.epilog

sw.epilog:                                        ; preds = %sw.default, %if.end9, %sw.bb
  %12 = load i32, ptr %result, align 4
  %cmp10 = icmp sgt i32 %12, 0
  %13 = zext i1 %cmp10 to i64
  %cond = select i1 %cmp10, i32 0, i32 1
  ret i32 %cond
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 18.1.8 (https://github.com/llvm/llvm-project.git 3b5b5c1ec4a3095ab096dd780e84d7ab81f3d7ff)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
