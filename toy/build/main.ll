; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%vec_of_int_4 = type { ptr, ptr, i64, i64 }
%map_pair = type { %map_key, %map_value }
%map_key = type { ptr, i64 }
%map_value = type { ptr, i64 }
%struct_map = type { ptr, i64, i64 }

@"map_key.a\00" = private constant [2 x i8] c"a\00"
@"map_key.b\00" = private constant [2 x i8] c"b\00"

define dso_local i32 @main() !lynx.signature !0 {
entry:
  %vector_data_heap = call ptr @GC_malloc(i64 16)
  %0 = getelementptr inbounds i32, ptr %vector_data_heap, i64 0
  store i32 1, ptr %0, align 4
  %1 = getelementptr inbounds i32, ptr %vector_data_heap, i64 1
  store i32 2, ptr %1, align 4
  %2 = getelementptr inbounds i32, ptr %vector_data_heap, i64 2
  store i32 3, ptr %2, align 4
  %3 = getelementptr inbounds i32, ptr %vector_data_heap, i64 3
  store i32 4, ptr %3, align 4
  %vector_struct_heap = call ptr @GC_malloc(i64 32)
  %vec.data = getelementptr inbounds nuw %vec_of_int_4, ptr %vector_struct_heap, i32 0, i32 0
  store ptr %vector_data_heap, ptr %vec.data, align 8
  %vec.size = getelementptr inbounds nuw %vec_of_int_4, ptr %vector_struct_heap, i32 0, i32 1
  store i64 4, ptr %vec.size, align 4
  %vec.cap = getelementptr inbounds nuw %vec_of_int_4, ptr %vector_struct_heap, i32 0, i32 2
  store i64 4, ptr %vec.cap, align 4
  %vector_data_heap1 = call ptr @GC_malloc(i64 16)
  %4 = getelementptr inbounds i32, ptr %vector_data_heap1, i64 0
  store i32 5, ptr %4, align 4
  %5 = getelementptr inbounds i32, ptr %vector_data_heap1, i64 1
  store i32 6, ptr %5, align 4
  %6 = getelementptr inbounds i32, ptr %vector_data_heap1, i64 2
  store i32 7, ptr %6, align 4
  %7 = getelementptr inbounds i32, ptr %vector_data_heap1, i64 3
  store i32 8, ptr %7, align 4
  %vector_struct_heap2 = call ptr @GC_malloc(i64 32)
  %vec.data3 = getelementptr inbounds nuw %vec_of_int_4, ptr %vector_struct_heap2, i32 0, i32 0
  store ptr %vector_data_heap1, ptr %vec.data3, align 8
  %vec.size4 = getelementptr inbounds nuw %vec_of_int_4, ptr %vector_struct_heap2, i32 0, i32 1
  store i64 4, ptr %vec.size4, align 4
  %vec.cap5 = getelementptr inbounds nuw %vec_of_int_4, ptr %vector_struct_heap2, i32 0, i32 2
  store i64 4, ptr %vec.cap5, align 4
  %pairs_heap = call ptr @GC_malloc(i64 64)
  %pair_elem_ptr = getelementptr inbounds %map_pair, ptr %pairs_heap, i64 0
  %elem.key = getelementptr inbounds nuw %map_pair, ptr %pair_elem_ptr, i32 0, i32 0
  %8 = getelementptr inbounds nuw %map_key, ptr %elem.key, i32 0, i32 0
  store ptr @"map_key.a\00", ptr %8, align 8
  %9 = getelementptr inbounds nuw %map_key, ptr %elem.key, i32 0, i32 1
  store i64 2, ptr %9, align 4
  %elem.val = getelementptr inbounds nuw %map_pair, ptr %pair_elem_ptr, i32 0, i32 1
  %10 = getelementptr inbounds nuw %map_value, ptr %elem.val, i32 0, i32 0
  store ptr %vector_struct_heap, ptr %10, align 8
  %11 = getelementptr inbounds nuw %map_value, ptr %elem.val, i32 0, i32 1
  store i64 0, ptr %11, align 4
  %pair_elem_ptr6 = getelementptr inbounds %map_pair, ptr %pairs_heap, i64 1
  %elem.key7 = getelementptr inbounds nuw %map_pair, ptr %pair_elem_ptr6, i32 0, i32 0
  %12 = getelementptr inbounds nuw %map_key, ptr %elem.key7, i32 0, i32 0
  store ptr @"map_key.b\00", ptr %12, align 8
  %13 = getelementptr inbounds nuw %map_key, ptr %elem.key7, i32 0, i32 1
  store i64 2, ptr %13, align 4
  %elem.val8 = getelementptr inbounds nuw %map_pair, ptr %pair_elem_ptr6, i32 0, i32 1
  %14 = getelementptr inbounds nuw %map_value, ptr %elem.val8, i32 0, i32 0
  store ptr %vector_struct_heap2, ptr %14, align 8
  %15 = getelementptr inbounds nuw %map_value, ptr %elem.val8, i32 0, i32 1
  store i64 0, ptr %15, align 4
  %map_heap = call ptr @GC_malloc(i64 24)
  %16 = getelementptr inbounds nuw %struct_map, ptr %map_heap, i32 0, i32 0
  store ptr %pairs_heap, ptr %16, align 8
  %17 = getelementptr inbounds nuw %struct_map, ptr %map_heap, i32 0, i32 1
  store i64 2, ptr %17, align 4
  %18 = getelementptr inbounds nuw %struct_map, ptr %map_heap, i32 0, i32 2
  store i64 2, ptr %18, align 4
  ret i32 0
}

declare ptr @GC_malloc(i64)

!0 = !{!"main(int)"}
