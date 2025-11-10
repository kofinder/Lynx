; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%lynx.char.type = type { i8 }

@lynx.string.constant = private constant [23 x i8] c"Enter a line of text: \00"
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@read_line_fmt = private unnamed_addr constant [10 x i8] c"%1023[^\0A]\00", align 1
@lynx.string.constant.2 = private constant [21 x i8] c"Enter a byte value: \00"
@fmt.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@fmt.4 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@scanf_fmt = private unnamed_addr constant [5 x i8] c"%hhd\00", align 1
@lynx.string.constant.5 = private constant [22 x i8] c"Enter a short value: \00"
@fmt.6 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@fmt.7 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@scanf_fmt.8 = private unnamed_addr constant [4 x i8] c"%hd\00", align 1
@lynx.string.constant.9 = private constant [21 x i8] c"Enter an int value: \00"
@fmt.10 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@fmt.11 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@scanf_fmt.12 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@lynx.string.constant.13 = private constant [21 x i8] c"Enter a long value: \00"
@fmt.14 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@fmt.15 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@scanf_fmt.16 = private unnamed_addr constant [4 x i8] c"%ld\00", align 1
@lynx.string.constant.17 = private constant [22 x i8] c"Enter a float value: \00"
@fmt.18 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@fmt.19 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@scanf_fmt.20 = private unnamed_addr constant [3 x i8] c"%f\00", align 1
@lynx.string.constant.21 = private constant [23 x i8] c"Enter a double value: \00"
@fmt.22 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@fmt.23 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@scanf_fmt.24 = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@lynx.string.constant.25 = private constant [21 x i8] c"Enter a char value: \00"
@fmt.26 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@fmt.27 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@scanf_fmt.28 = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@lynx.string.constant.29 = private constant [46 x i8] c"Enter a boolean value (0 = false, 1 = true): \00"
@fmt.30 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@fmt.31 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@scanf_fmt.32 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@lynx.string.constant.33 = private constant [106 x i8] c"You entered:\\nLine: {}\\nByte: {}\\nShort: {}\\nInt: {}\\nLong: {}\\nFloat: {}\\nDouble: {}\\nChar: {}\\nBool: {}\00"
@fmt.34 = private unnamed_addr constant [108 x i8] c"You entered:\\nLine: %s\\nByte: %d\\nShort: %d\\nInt: %d\\nLong: %ld\\nFloat: %f\\nDouble: %lf\\nChar: %c\\nBool: %s\00", align 1
@fmt.35 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@fmt.36 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@fmt.37 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@fmt.38 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@fmt.39 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@fmt.40 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@fmt.41 = private unnamed_addr constant [5 x i8] c"%lf\0A\00", align 1
@fmt.42 = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1
@fmt.43 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@0 = private unnamed_addr constant [6 x i8] c"false\00", align 1
@1 = private unnamed_addr constant [5 x i8] c"true\00", align 1

; Function Attrs: alwaysinline norecurse uwtable
define dso_local signext i32 @main() #0 !lynx.signature !0 {
entry:
  %io_printf_call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @lynx.string.constant, i32 0, i32 0))
  %input_buffer = alloca i8, i32 1024, align 1
  %scanf_read_line = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @read_line_fmt, i32 0, i32 0), i8* %input_buffer)
  %line = alloca i8*, align 8, !lynx.type !1
  store i8* %input_buffer, i8** %line, align 8
  %io_printf_call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.3, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @lynx.string.constant.2, i32 0, i32 0))
  %scanf_tmp = alloca i32, align 4
  %scanf_call = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @scanf_fmt, i32 0, i32 0), i32* %scanf_tmp)
  %read_value = load i32, i32* %scanf_tmp, align 4
  %b = alloca i32, align 4, !lynx.type !2
  store i32 %read_value, i32* %b, align 4
  %io_printf_call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @lynx.string.constant.5, i32 0, i32 0))
  %scanf_tmp3 = alloca i16, align 2
  %scanf_call4 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @scanf_fmt.8, i32 0, i32 0), i16* %scanf_tmp3)
  %read_value5 = load i16, i16* %scanf_tmp3, align 2
  %s = alloca i16, align 2, !lynx.type !3
  store i16 %read_value5, i16* %s, align 2
  %io_printf_call6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @lynx.string.constant.9, i32 0, i32 0))
  %scanf_tmp7 = alloca i32, align 4
  %scanf_call8 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @scanf_fmt.12, i32 0, i32 0), i32* %scanf_tmp7)
  %read_value9 = load i32, i32* %scanf_tmp7, align 4
  %i = alloca i32, align 4, !lynx.type !2
  store i32 %read_value9, i32* %i, align 4
  %io_printf_call10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.14, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @lynx.string.constant.13, i32 0, i32 0))
  %scanf_tmp11 = alloca i64, align 8
  %scanf_call12 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @scanf_fmt.16, i32 0, i32 0), i64* %scanf_tmp11)
  %read_value13 = load i64, i64* %scanf_tmp11, align 4
  %l = alloca i64, align 8, !lynx.type !4
  store i64 %read_value13, i64* %l, align 4
  %io_printf_call14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.18, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @lynx.string.constant.17, i32 0, i32 0))
  %scanf_tmp15 = alloca float, align 4
  %scanf_call16 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @scanf_fmt.20, i32 0, i32 0), float* %scanf_tmp15)
  %read_value17 = load float, float* %scanf_tmp15, align 4
  %f = alloca float, align 4, !lynx.type !5
  store float %read_value17, float* %f, align 4
  %io_printf_call18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.22, i32 0, i32 0), i8* getelementptr inbounds ([23 x i8], [23 x i8]* @lynx.string.constant.21, i32 0, i32 0))
  %scanf_tmp19 = alloca double, align 8
  %scanf_call20 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @scanf_fmt.24, i32 0, i32 0), double* %scanf_tmp19)
  %read_value21 = load double, double* %scanf_tmp19, align 8
  %d = alloca double, align 8, !lynx.type !6
  store double %read_value21, double* %d, align 8
  %io_printf_call22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.26, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8], [21 x i8]* @lynx.string.constant.25, i32 0, i32 0))
  %scanf_tmp23 = alloca %lynx.char.type, align 8
  %scanf_call24 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @scanf_fmt.28, i32 0, i32 0), %lynx.char.type* %scanf_tmp23)
  %read_value25 = load %lynx.char.type, %lynx.char.type* %scanf_tmp23, align 1
  %c = alloca %lynx.char.type, align 8, !lynx.type !7
  store %lynx.char.type %read_value25, %lynx.char.type* %c, align 1
  %io_printf_call26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.30, i32 0, i32 0), i8* getelementptr inbounds ([46 x i8], [46 x i8]* @lynx.string.constant.29, i32 0, i32 0))
  %bool_tmp = alloca i32, align 4
  %scanf_tmp27 = alloca i32, align 4
  %scanf_call28 = call i32 (i8*, ...) @scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @scanf_fmt.32, i32 0, i32 0), i32* %scanf_tmp27)
  %read_value29 = load i32, i32* %scanf_tmp27, align 4
  store i32 %read_value29, i32* %bool_tmp, align 4
  %bool_loaded = load i32, i32* %bool_tmp, align 4
  %bool_val = icmp ne i32 %bool_loaded, 0
  %flag = alloca i1, align 1, !lynx.type !8
  store i1 %bool_val, i1* %flag, align 1
  %line_load = load i8*, i8** %line, align 8
  %b_load = load i32, i32* %b, align 4
  %s_load = load i16, i16* %s, align 2
  %i_load = load i32, i32* %i, align 4
  %l_load = load i64, i64* %l, align 4
  %f_load = load float, float* %f, align 4
  %d_load = load double, double* %d, align 8
  %flag_load = load i1, i1* %flag, align 1
  %promotedFloat = fpext float %f_load to double
  %char_field_ptr = getelementptr inbounds %lynx.char.type, %lynx.char.type* %c, i32 0, i32 0
  %char_load_val = load i8, i8* %char_field_ptr, align 1
  %char_promoted = sext i8 %char_load_val to i32
  %0 = select i1 %flag_load, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @0, i32 0, i32 0)
  %printf_call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([108 x i8], [108 x i8]* @fmt.34, i32 0, i32 0), i8* %line_load, i32 %b_load, i16 %s_load, i32 %i_load, i64 %l_load, double %promotedFloat, double %d_load, i32 %char_promoted, i8* %0)
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare i32 @scanf(i8*, ...)

attributes #0 = { alwaysinline norecurse uwtable }

!0 = !{!"main(int)"}
!1 = !{!"lynx.string.type"}
!2 = !{!"lynx.int.type"}
!3 = !{!"lynx.short.type"}
!4 = !{!"lynx.long.type"}
!5 = !{!"lynx.float.type"}
!6 = !{!"lynx.double.type"}
!7 = !{!"lynx.char.type"}
!8 = !{!"lynx.boolean.type"}
