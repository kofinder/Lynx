; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%class.ChatUser = type { i8*, i1, i32, i8* }

@import_src_user_chat_user = private constant [19 x i8] c"src/user/chat_user\00"
@lynx.string.constant = private constant [6 x i8] c"id123\00"
@lynx.string.constant.1 = private constant [9 x i8] c"Ko Thein\00"

; Function Attrs: alwaysinline norecurse uwtable
define dso_local signext i32 @main() #0 !lynx.signature !0 {
entry:
  %gc_alloc = call i8* @LYNX_GC_ALLOC_chatuser(i64 24)
  %gc_cast = bitcast i8* %gc_alloc to %class.ChatUser*
  call void @_ZN8ChatUserC1ESsSsib(%class.ChatUser* %gc_cast, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @lynx.string.constant, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @lynx.string.constant.1, i32 0, i32 0), i32 30, i1 true)
  ret i32 0
}

declare i8* @LYNX_GC_ALLOC_chatuser(i64)

declare void @_ZN8ChatUserC1ESsSsib(%class.ChatUser*, i8*, i8*, i32, i1)

attributes #0 = { alwaysinline norecurse uwtable }

!0 = !{!"main(int)"}
