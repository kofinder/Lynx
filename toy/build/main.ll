; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@lynx.string.constant = private constant [13 x i8] c"HELLO WORLD!\00"
@fmt = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

; Function Attrs: alwaysinline norecurse uwtable
define dso_local signext i32 @main() #0 !lynx.signature !0 {
entry:
  %io_printf_call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @lynx.string.constant, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

attributes #0 = { alwaysinline norecurse uwtable }

!0 = !{!"main(int)"}
