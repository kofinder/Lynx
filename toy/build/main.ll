; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%vec_nested_2_of_vec_recursive = type { ptr, ptr, i64, i64 }

@const_vector = private constant [2 x %vec_nested_2_of_vec_recursive] [%vec_nested_2_of_vec_recursive [i32 1000, i32 2000], %vec_nested_2_of_vec_recursive [i32 5000, i32 6000]], align 4

define dso_local i32 @main() !lynx.signature !0 {
entry:
  %vec = alloca %vec_nested_2_of_vec_recursive, align 32, !lynx.type !1
  %gc_malloc = call ptr @GC_malloc(i64 64)
  call void @llvm.memcpy.p0.p0.i64(ptr %gc_malloc, ptr @const_vector, i64 64, i1 false)
  %vec.data = getelementptr inbounds nuw %vec_nested_2_of_vec_recursive, ptr %vec, i32 0, i32 0
  store ptr %gc_malloc, ptr %vec.data, align 8
  %vec.size = getelementptr inbounds nuw %vec_nested_2_of_vec_recursive, ptr %vec, i32 0, i32 1
  store i64 2, ptr %vec.size, align 4
  %vec.cap = getelementptr inbounds nuw %vec_nested_2_of_vec_recursive, ptr %vec, i32 0, i32 2
  store i64 2, ptr %vec.cap, align 4
  ret i32 0
}

declare ptr @GC_malloc(i64)

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #0

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!0 = !{!"main(int)"}
!1 = !{!"lynx.vector.type"}
