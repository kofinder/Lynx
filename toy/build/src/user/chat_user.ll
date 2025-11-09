; ModuleID = 'chat_user'
source_filename = "chat_user"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%class.ChatUser = type { i8*, i1, i32, i8* }

$_ZN8ChatUserC1ESsSsib = comdat any

$_ZN8ChatUser5getIdEV = comdat any

$_ZN8ChatUser5setIdESs = comdat any

$_ZN8ChatUser7getNameEV = comdat any

$_ZN8ChatUser7setNameESs = comdat any

$_ZN8ChatUser6getAgeEV = comdat any

$_ZN8ChatUser6setAgeEi = comdat any

$_ZN8ChatUser9getOnlineEV = comdat any

$_ZN8ChatUser8setIsBotEb = comdat any

; Function Attrs: noinline nounwind uwtable
define linkonce_odr dso_local void @_ZN8ChatUserC1ESsSsib(%class.ChatUser* noalias nonnull %this, i8* %id, i8* %name, i32 %age, i1 %isBot) #0 comdat {
entry:
  %id_ptr = getelementptr inbounds %class.ChatUser, %class.ChatUser* %this, i32 0, i32 3
  store i8* %id, i8** %id_ptr, align 8
  %name_ptr = getelementptr inbounds %class.ChatUser, %class.ChatUser* %this, i32 0, i32 0
  store i8* %name, i8** %name_ptr, align 8
  %age_ptr = getelementptr inbounds %class.ChatUser, %class.ChatUser* %this, i32 0, i32 2
  store i32 %age, i32* %age_ptr, align 4
  %isBot_ptr = getelementptr inbounds %class.ChatUser, %class.ChatUser* %this, i32 0, i32 1
  store i1 %isBot, i1* %isBot_ptr, align 1
  ret void
}

define linkonce_odr dso_local i8* @_ZN8ChatUser5getIdEV(%class.ChatUser* %this) #1 comdat {
entry:
  %id_ptr = getelementptr inbounds %class.ChatUser, %class.ChatUser* %this, i32 0, i32 3
  %id_val = load i8*, i8** %id_ptr, align 8
  ret i8* %id_val
}

define linkonce_odr dso_local void @_ZN8ChatUser5setIdESs(%class.ChatUser* %this, i8* %id) #1 comdat {
entry:
  %id_ptr = getelementptr inbounds %class.ChatUser, %class.ChatUser* %this, i32 0, i32 3
  store i8* %id, i8** %id_ptr, align 8
  ret void
}

define linkonce_odr dso_local i8* @_ZN8ChatUser7getNameEV(%class.ChatUser* %this) #1 comdat {
entry:
  %name_ptr = getelementptr inbounds %class.ChatUser, %class.ChatUser* %this, i32 0, i32 0
  %name_val = load i8*, i8** %name_ptr, align 8
  ret i8* %name_val
}

define linkonce_odr dso_local void @_ZN8ChatUser7setNameESs(%class.ChatUser* %this, i8* %name) #1 comdat {
entry:
  %name_ptr = getelementptr inbounds %class.ChatUser, %class.ChatUser* %this, i32 0, i32 0
  store i8* %name, i8** %name_ptr, align 8
  ret void
}

define linkonce_odr dso_local i32 @_ZN8ChatUser6getAgeEV(%class.ChatUser* %this) #1 comdat {
entry:
  %age_ptr = getelementptr inbounds %class.ChatUser, %class.ChatUser* %this, i32 0, i32 2
  %age_val = load i32, i32* %age_ptr, align 4
  ret i32 %age_val
}

define linkonce_odr dso_local void @_ZN8ChatUser6setAgeEi(%class.ChatUser* %this, i32 %age) #1 comdat {
entry:
  %age_ptr = getelementptr inbounds %class.ChatUser, %class.ChatUser* %this, i32 0, i32 2
  store i32 %age, i32* %age_ptr, align 4
  ret void
}

define linkonce_odr dso_local i1 @_ZN8ChatUser9getOnlineEV(%class.ChatUser* %this) #1 comdat {
entry:
  %isBot_ptr = getelementptr inbounds %class.ChatUser, %class.ChatUser* %this, i32 0, i32 1
  %isBot_val = load i1, i1* %isBot_ptr, align 1
  ret i1 %isBot_val
}

define linkonce_odr dso_local void @_ZN8ChatUser8setIsBotEb(%class.ChatUser* %this, i1 %isBot) #1 comdat {
entry:
  %isBot_ptr = getelementptr inbounds %class.ChatUser, %class.ChatUser* %this, i32 0, i32 1
  store i1 %isBot, i1* %isBot_ptr, align 1
  ret void
}

attributes #0 = { noinline nounwind uwtable }
attributes #1 = { "access_modifier"="public" }

!lynx.fields = !{!0}
!lynx.methods = !{!5, !7}

!0 = !{!1, !2, !3, !4}
!1 = !{!"ChatUser", !"private", !"string", !"id"}
!2 = !{!"ChatUser", !"private", !"int", !"age"}
!3 = !{!"ChatUser", !"private", !"string", !"name"}
!4 = !{!"ChatUser", !"private", !"boolean", !"isBot"}
!5 = !{!6}
!6 = !{!"ChatUser", !"public", !"ctor"}
!7 = !{!8, !9, !10, !11, !12, !13, !14, !15}
!8 = !{!"ChatUser", !"public", !"string", !"_ZN8ChatUser5getIdEV", !"getId(string)"}
!9 = !{!"ChatUser", !"public", !"void", !"_ZN8ChatUser5setIdESs", !"setId(void, string)"}
!10 = !{!"ChatUser", !"public", !"string", !"_ZN8ChatUser7getNameEV", !"getName(string)"}
!11 = !{!"ChatUser", !"public", !"void", !"_ZN8ChatUser7setNameESs", !"setName(void, string)"}
!12 = !{!"ChatUser", !"public", !"int", !"_ZN8ChatUser6getAgeEV", !"getAge(int)"}
!13 = !{!"ChatUser", !"public", !"void", !"_ZN8ChatUser6setAgeEi", !"setAge(void, int)"}
!14 = !{!"ChatUser", !"public", !"boolean", !"_ZN8ChatUser9getOnlineEV", !"getOnline(boolean)"}
!15 = !{!"ChatUser", !"public", !"void", !"_ZN8ChatUser8setIsBotEb", !"setIsBot(void, boolean)"}
