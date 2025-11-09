; ModuleID = 'main'
source_filename = "main"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%0 = type { i8*, i1, i32, i8* }

$_ZN8ChatUserC1ESsSsib = comdat any

@import_src_user_chat_user = private constant [19 x i8] c"src/user/chat_user\00"
@lynx.string.constant = private constant [6 x i8] c"id123\00"
@lynx.string.constant.1 = private constant [9 x i8] c"Ko Thein\00"

; Function Attrs: alwaysinline norecurse uwtable
define dso_local signext i32 @main() #0 !lynx.signature !9 {
entry:
  %gc_alloc = call i8* @LYNX_GC_ALLOC_chatuser(i64 24)
  %gc_cast = bitcast i8* %gc_alloc to %0*
  call void @_ZN8ChatUserC1ESsSsib(%0* %gc_cast, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @lynx.string.constant, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @lynx.string.constant.1, i32 0, i32 0), i32 30, i1 true)
  ret i32 0
}

declare i8* @LYNX_GC_ALLOC_chatuser(i64)

declare i8* @GC_malloc(i64)

declare i32 @pthread_create(i8**, i8**, i8* (i8*)*, i8*)

declare i32 @pthread_join(i64, i8**)

declare i64 @pthread_self()

declare i32 @pthread_equal(i64, i64)

; Function Attrs: noinline nounwind uwtable
define linkonce_odr dso_local void @_ZN8ChatUserC1ESsSsib(%0* noalias nonnull %this, i8* %id, i8* %name, i32 %age, i1 %isBot) #1 comdat {
entry:
  %id_ptr = getelementptr inbounds %0, %0* %this, i32 0, i32 3
  store i8* %id, i8** %id_ptr, align 8
  %name_ptr = getelementptr inbounds %0, %0* %this, i32 0, i32 0
  store i8* %name, i8** %name_ptr, align 8
  %age_ptr = getelementptr inbounds %0, %0* %this, i32 0, i32 2
  store i32 %age, i32* %age_ptr, align 4
  %isBot_ptr = getelementptr inbounds %0, %0* %this, i32 0, i32 1
  store i1 %isBot, i1* %isBot_ptr, align 1
  ret void
}

attributes #0 = { alwaysinline norecurse uwtable }
attributes #1 = { noinline nounwind uwtable }

!lynx.fields = !{!0}
!lynx.methods = !{!5, !7}

!0 = !{!1, !2, !3, !4}
!1 = !{!"ChatUser", !"private", !"string", !"id"}
!2 = !{!"ChatUser", !"private", !"int", !"age"}
!3 = !{!"ChatUser", !"private", !"string", !"name"}
!4 = !{!"ChatUser", !"private", !"boolean", !"isBot"}
!5 = !{!6}
!6 = !{!"ChatUser", !"public", !"ctor"}
!7 = !{!8}
!8 = !{!"ChatUser", !"public", !"string", !"_ZN8ChatUser5getIdEV", !"getId(string)"}
!9 = !{!"main(int)"}
