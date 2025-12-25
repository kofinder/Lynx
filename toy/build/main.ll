; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%map_key = type { ptr, i64 }
%map_pair = type { ptr, ptr }
%struct_map = type { ptr, i64, i64 }

@str_a = private constant [2 x i8] c"a\00"
@map_key_str_a = private constant %map_key { ptr @str_a, i64 0 }
@str_b = private constant [2 x i8] c"b\00"
@map_key_str_b = private constant %map_key { ptr @str_b, i64 0 }
@map_key.0 = private constant [1 x %map_key] [%map_key @map_key_str_a]
@map_key.1 = private constant [1 x %map_key] [%map_key @map_key_str_b]
@map_value.0 = private constant i32 1
@map_value.1 = private constant i32 2
@flat_array_const = private constant [2 x %map_pair] [%map_pair { ptr @map_key.0, ptr @map_value.0 }, %map_pair { ptr @map_key.1, ptr @map_value.1 }]

define dso_local i32 @main() !lynx.signature !0 {
entry:
  %flat_array_raw = call ptr @GC_malloc(i64 32)
  call void @llvm.memcpy.p0.p0.i64(ptr %flat_array_raw, ptr @flat_array_const, i64 32, i1 false)
  %map_raw = call ptr @GC_malloc(i64 24)
  %ptr_gep = getelementptr inbounds nuw %struct_map, ptr %map_raw, i32 0, i32 0
  %length_gep = getelementptr inbounds nuw %struct_map, ptr %map_raw, i32 0, i32 1
  %cap_gep = getelementptr inbounds nuw %struct_map, ptr %map_raw, i32 0, i32 2
  store ptr %flat_array_raw, ptr %ptr_gep, align 8
  store i64 2, ptr %length_gep, align 4
  store i64 2, ptr %cap_gep, align 4
  ret i32 0
}

declare ptr @GC_malloc(i64)

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #0

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!0 = !{!"main(int)"}
