; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@lynx.string.constant = private constant [18 x i8] c"Enter your name: \00"
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@formatString = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@read_line_fmt = private unnamed_addr constant [10 x i8] c"%1023[^\0A]\00", align 1
@lynx.string.constant.1 = private constant [17 x i8] c"Enter your age: \00"
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@formatString.3 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@scanf_fmt = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@lynx.string.constant.4 = private constant [18 x i8] c"Name: {}, Age: {}\00"
@fmt.5 = private unnamed_addr constant [10 x i8] c"%s %s %d\0A\00", align 1
@formatString.6 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@formatString.7 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@formatString.8 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: alwaysinline norecurse uwtable
define dso_local signext i32 @main() #0 !lynx.signature !0 {
entry:
  %io_printf_call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @lynx.string.constant, i32 0, i32 0))
  %input_buffer = alloca i8, i32 1024, align 1
  %scanf_read_line = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @read_line_fmt, i32 0, i32 0), i8* %input_buffer)
  %name = alloca i8*, align 8, !lynx.type !1
  store i8* %input_buffer, i8** %name, align 8
  %io_printf_call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @lynx.string.constant.1, i32 0, i32 0))
  %scanf_tmp = alloca i32, align 4
  %scanfCall = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @scanf_fmt, i32 0, i32 0), i32* %scanf_tmp)
  %read_value = load i32, i32* %scanf_tmp, align 4
  %age = alloca i32, align 4, !lynx.type !2
  store i32 %read_value, i32* %age, align 4
  %name_load = load i8*, i8** %name, align 8
  %age_load = load i32, i32* %age, align 4
  %io_printf_call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([18 x i8], [18 x i8]* @lynx.string.constant.4, i32 0, i32 0), i8* %name_load, i32 %age_load)
  ret i32 32
}

declare i32 @printf(i8*, ...)

declare i32 @scanf(i8*, ...)

attributes #0 = { alwaysinline norecurse uwtable }

!0 = !{!"main(int)"}
!1 = !{!"lynx.string.type"}
!2 = !{!"lynx.int.type"}
