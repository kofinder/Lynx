
dump/myprogram:     file format elf64-x86-64


Disassembly of section .init:

0000000000401000 <_init>:
  401000:	f3 0f 1e fa          	endbr64 
  401004:	48 83 ec 08          	sub    $0x8,%rsp
  401008:	48 8b 05 d1 2f 00 00 	mov    0x2fd1(%rip),%rax        # 403fe0 <__gmon_start__>
  40100f:	48 85 c0             	test   %rax,%rax
  401012:	74 02                	je     401016 <_init+0x16>
  401014:	ff d0                	callq  *%rax
  401016:	48 83 c4 08          	add    $0x8,%rsp
  40101a:	c3                   	retq   

Disassembly of section .plt:

0000000000401020 <.plt>:
  401020:	ff 35 e2 2f 00 00    	pushq  0x2fe2(%rip)        # 404008 <_GLOBAL_OFFSET_TABLE_+0x8>
  401026:	ff 25 e4 2f 00 00    	jmpq   *0x2fe4(%rip)        # 404010 <_GLOBAL_OFFSET_TABLE_+0x10>
  40102c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000401030 <_ZSt9terminatev@plt>:
  401030:	ff 25 e2 2f 00 00    	jmpq   *0x2fe2(%rip)        # 404018 <_ZSt9terminatev@GLIBCXX_3.4>
  401036:	68 00 00 00 00       	pushq  $0x0
  40103b:	e9 e0 ff ff ff       	jmpq   401020 <.plt>

0000000000401040 <_ZNSt6thread6_StateD2Ev@plt>:
  401040:	ff 25 da 2f 00 00    	jmpq   *0x2fda(%rip)        # 404020 <_ZNSt6thread6_StateD2Ev@GLIBCXX_3.4.22>
  401046:	68 01 00 00 00       	pushq  $0x1
  40104b:	e9 d0 ff ff ff       	jmpq   401020 <.plt>

0000000000401050 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@plt>:
  401050:	ff 25 d2 2f 00 00    	jmpq   *0x2fd2(%rip)        # 404028 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GLIBCXX_3.4>
  401056:	68 02 00 00 00       	pushq  $0x2
  40105b:	e9 c0 ff ff ff       	jmpq   401020 <.plt>

0000000000401060 <pthread_create@plt>:
  401060:	ff 25 ca 2f 00 00    	jmpq   *0x2fca(%rip)        # 404030 <pthread_create@GLIBC_2.2.5>
  401066:	68 03 00 00 00       	pushq  $0x3
  40106b:	e9 b0 ff ff ff       	jmpq   401020 <.plt>

0000000000401070 <__cxa_atexit@plt>:
  401070:	ff 25 c2 2f 00 00    	jmpq   *0x2fc2(%rip)        # 404038 <__cxa_atexit@GLIBC_2.2.5>
  401076:	68 04 00 00 00       	pushq  $0x4
  40107b:	e9 a0 ff ff ff       	jmpq   401020 <.plt>

0000000000401080 <_ZNSt6thread15_M_start_threadESt10unique_ptrINS_6_StateESt14default_deleteIS1_EEPFvvE@plt>:
  401080:	ff 25 ba 2f 00 00    	jmpq   *0x2fba(%rip)        # 404040 <_ZNSt6thread15_M_start_threadESt10unique_ptrINS_6_StateESt14default_deleteIS1_EEPFvvE@GLIBCXX_3.4.22>
  401086:	68 05 00 00 00       	pushq  $0x5
  40108b:	e9 90 ff ff ff       	jmpq   401020 <.plt>

0000000000401090 <_Znwm@plt>:
  401090:	ff 25 b2 2f 00 00    	jmpq   *0x2fb2(%rip)        # 404048 <_Znwm@GLIBCXX_3.4>
  401096:	68 06 00 00 00       	pushq  $0x6
  40109b:	e9 80 ff ff ff       	jmpq   401020 <.plt>

00000000004010a0 <_ZdlPvm@plt>:
  4010a0:	ff 25 aa 2f 00 00    	jmpq   *0x2faa(%rip)        # 404050 <_ZdlPvm@CXXABI_1.3.9>
  4010a6:	68 07 00 00 00       	pushq  $0x7
  4010ab:	e9 70 ff ff ff       	jmpq   401020 <.plt>

00000000004010b0 <_ZNSolsEPFRSoS_E@plt>:
  4010b0:	ff 25 a2 2f 00 00    	jmpq   *0x2fa2(%rip)        # 404058 <_ZNSolsEPFRSoS_E@GLIBCXX_3.4>
  4010b6:	68 08 00 00 00       	pushq  $0x8
  4010bb:	e9 60 ff ff ff       	jmpq   401020 <.plt>

00000000004010c0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c@plt>:
  4010c0:	ff 25 9a 2f 00 00    	jmpq   *0x2f9a(%rip)        # 404060 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c@GLIBCXX_3.4>
  4010c6:	68 09 00 00 00       	pushq  $0x9
  4010cb:	e9 50 ff ff ff       	jmpq   401020 <.plt>

00000000004010d0 <_ZSt5flushIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@plt>:
  4010d0:	ff 25 92 2f 00 00    	jmpq   *0x2f92(%rip)        # 404068 <_ZSt5flushIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GLIBCXX_3.4>
  4010d6:	68 0a 00 00 00       	pushq  $0xa
  4010db:	e9 40 ff ff ff       	jmpq   401020 <.plt>

00000000004010e0 <_ZNSt8ios_base4InitC1Ev@plt>:
  4010e0:	ff 25 8a 2f 00 00    	jmpq   *0x2f8a(%rip)        # 404070 <_ZNSt8ios_base4InitC1Ev@GLIBCXX_3.4>
  4010e6:	68 0b 00 00 00       	pushq  $0xb
  4010eb:	e9 30 ff ff ff       	jmpq   401020 <.plt>

00000000004010f0 <__gxx_personality_v0@plt>:
  4010f0:	ff 25 82 2f 00 00    	jmpq   *0x2f82(%rip)        # 404078 <__gxx_personality_v0@CXXABI_1.3>
  4010f6:	68 0c 00 00 00       	pushq  $0xc
  4010fb:	e9 20 ff ff ff       	jmpq   401020 <.plt>

0000000000401100 <_ZNSt6thread4joinEv@plt>:
  401100:	ff 25 7a 2f 00 00    	jmpq   *0x2f7a(%rip)        # 404080 <_ZNSt6thread4joinEv@GLIBCXX_3.4.11>
  401106:	68 0d 00 00 00       	pushq  $0xd
  40110b:	e9 10 ff ff ff       	jmpq   401020 <.plt>

0000000000401110 <_Unwind_Resume@plt>:
  401110:	ff 25 72 2f 00 00    	jmpq   *0x2f72(%rip)        # 404088 <_Unwind_Resume@GCC_3.0>
  401116:	68 0e 00 00 00       	pushq  $0xe
  40111b:	e9 00 ff ff ff       	jmpq   401020 <.plt>

0000000000401120 <_ZNSt8ios_base4InitD1Ev@plt>:
  401120:	ff 25 6a 2f 00 00    	jmpq   *0x2f6a(%rip)        # 404090 <_ZNSt8ios_base4InitD1Ev@GLIBCXX_3.4>
  401126:	68 0f 00 00 00       	pushq  $0xf
  40112b:	e9 f0 fe ff ff       	jmpq   401020 <.plt>

Disassembly of section .text:

0000000000401130 <_start>:
  401130:	f3 0f 1e fa          	endbr64 
  401134:	31 ed                	xor    %ebp,%ebp
  401136:	49 89 d1             	mov    %rdx,%r9
  401139:	5e                   	pop    %rsi
  40113a:	48 89 e2             	mov    %rsp,%rdx
  40113d:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  401141:	50                   	push   %rax
  401142:	54                   	push   %rsp
  401143:	49 c7 c0 40 1a 40 00 	mov    $0x401a40,%r8
  40114a:	48 c7 c1 d0 19 40 00 	mov    $0x4019d0,%rcx
  401151:	48 c7 c7 66 12 40 00 	mov    $0x401266,%rdi
  401158:	ff 15 7a 2e 00 00    	callq  *0x2e7a(%rip)        # 403fd8 <__libc_start_main@GLIBC_2.2.5>
  40115e:	f4                   	hlt    
  40115f:	90                   	nop

0000000000401160 <_dl_relocate_static_pie>:
  401160:	f3 0f 1e fa          	endbr64 
  401164:	c3                   	retq   
  401165:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  40116c:	00 00 00 
  40116f:	90                   	nop

0000000000401170 <deregister_tm_clones>:
  401170:	b8 a8 40 40 00       	mov    $0x4040a8,%eax
  401175:	48 3d a8 40 40 00    	cmp    $0x4040a8,%rax
  40117b:	74 13                	je     401190 <deregister_tm_clones+0x20>
  40117d:	b8 00 00 00 00       	mov    $0x0,%eax
  401182:	48 85 c0             	test   %rax,%rax
  401185:	74 09                	je     401190 <deregister_tm_clones+0x20>
  401187:	bf a8 40 40 00       	mov    $0x4040a8,%edi
  40118c:	ff e0                	jmpq   *%rax
  40118e:	66 90                	xchg   %ax,%ax
  401190:	c3                   	retq   
  401191:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
  401198:	00 00 00 00 
  40119c:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004011a0 <register_tm_clones>:
  4011a0:	be a8 40 40 00       	mov    $0x4040a8,%esi
  4011a5:	48 81 ee a8 40 40 00 	sub    $0x4040a8,%rsi
  4011ac:	48 89 f0             	mov    %rsi,%rax
  4011af:	48 c1 ee 3f          	shr    $0x3f,%rsi
  4011b3:	48 c1 f8 03          	sar    $0x3,%rax
  4011b7:	48 01 c6             	add    %rax,%rsi
  4011ba:	48 d1 fe             	sar    %rsi
  4011bd:	74 11                	je     4011d0 <register_tm_clones+0x30>
  4011bf:	b8 00 00 00 00       	mov    $0x0,%eax
  4011c4:	48 85 c0             	test   %rax,%rax
  4011c7:	74 07                	je     4011d0 <register_tm_clones+0x30>
  4011c9:	bf a8 40 40 00       	mov    $0x4040a8,%edi
  4011ce:	ff e0                	jmpq   *%rax
  4011d0:	c3                   	retq   
  4011d1:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
  4011d8:	00 00 00 00 
  4011dc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004011e0 <__do_global_dtors_aux>:
  4011e0:	f3 0f 1e fa          	endbr64 
  4011e4:	80 3d e5 2f 00 00 00 	cmpb   $0x0,0x2fe5(%rip)        # 4041d0 <completed.0>
  4011eb:	75 13                	jne    401200 <__do_global_dtors_aux+0x20>
  4011ed:	55                   	push   %rbp
  4011ee:	48 89 e5             	mov    %rsp,%rbp
  4011f1:	e8 7a ff ff ff       	callq  401170 <deregister_tm_clones>
  4011f6:	c6 05 d3 2f 00 00 01 	movb   $0x1,0x2fd3(%rip)        # 4041d0 <completed.0>
  4011fd:	5d                   	pop    %rbp
  4011fe:	c3                   	retq   
  4011ff:	90                   	nop
  401200:	c3                   	retq   
  401201:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
  401208:	00 00 00 00 
  40120c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000401210 <frame_dummy>:
  401210:	f3 0f 1e fa          	endbr64 
  401214:	eb 8a                	jmp    4011a0 <register_tm_clones>

0000000000401216 <_ZZ4mainENKUlvE_clEv>:
  401216:	55                   	push   %rbp
  401217:	48 89 e5             	mov    %rsp,%rbp
  40121a:	48 83 ec 20          	sub    $0x20,%rsp
  40121e:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  401222:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
  401229:	eb 20                	jmp    40124b <_ZZ4mainENKUlvE_clEv+0x35>
  40122b:	be 2b 00 00 00       	mov    $0x2b,%esi
  401230:	bf c0 40 40 00       	mov    $0x4040c0,%edi
  401235:	e8 86 fe ff ff       	callq  4010c0 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c@plt>
  40123a:	be d0 10 40 00       	mov    $0x4010d0,%esi
  40123f:	48 89 c7             	mov    %rax,%rdi
  401242:	e8 69 fe ff ff       	callq  4010b0 <_ZNSolsEPFRSoS_E@plt>
  401247:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
  40124b:	81 7d fc e7 03 00 00 	cmpl   $0x3e7,-0x4(%rbp)
  401252:	7e d7                	jle    40122b <_ZZ4mainENKUlvE_clEv+0x15>
  401254:	be 50 10 40 00       	mov    $0x401050,%esi
  401259:	bf c0 40 40 00       	mov    $0x4040c0,%edi
  40125e:	e8 4d fe ff ff       	callq  4010b0 <_ZNSolsEPFRSoS_E@plt>
  401263:	90                   	nop
  401264:	c9                   	leaveq 
  401265:	c3                   	retq   

0000000000401266 <main>:
  401266:	55                   	push   %rbp
  401267:	48 89 e5             	mov    %rsp,%rbp
  40126a:	41 56                	push   %r14
  40126c:	41 55                	push   %r13
  40126e:	41 54                	push   %r12
  401270:	53                   	push   %rbx
  401271:	48 83 ec 10          	sub    $0x10,%rsp
  401275:	48 8d 5d d0          	lea    -0x30(%rbp),%rbx
  401279:	48 89 de             	mov    %rbx,%rsi
  40127c:	bf 08 00 00 00       	mov    $0x8,%edi
  401281:	e8 d4 03 00 00       	callq  40165a <_ZnwmPv>
  401286:	49 89 c4             	mov    %rax,%r12
  401289:	41 be 01 00 00 00    	mov    $0x1,%r14d
  40128f:	48 8d 45 df          	lea    -0x21(%rbp),%rax
  401293:	48 89 c6             	mov    %rax,%rsi
  401296:	4c 89 e7             	mov    %r12,%rdi
  401299:	e8 4a 00 00 00       	callq  4012e8 <_ZNSt6threadC1IZ4mainEUlvE_JEvEEOT_DpOT0_>
  40129e:	48 8d 45 d0          	lea    -0x30(%rbp),%rax
  4012a2:	48 89 c7             	mov    %rax,%rdi
  4012a5:	e8 56 fe ff ff       	callq  401100 <_ZNSt6thread4joinEv@plt>
  4012aa:	48 8d 45 d0          	lea    -0x30(%rbp),%rax
  4012ae:	48 89 c7             	mov    %rax,%rdi
  4012b1:	e8 dc 03 00 00       	callq  401692 <_ZNSt6threadD1Ev>
  4012b6:	b8 00 00 00 00       	mov    $0x0,%eax
  4012bb:	eb 1e                	jmp    4012db <main+0x75>
  4012bd:	49 89 c5             	mov    %rax,%r13
  4012c0:	45 84 f6             	test   %r14b,%r14b
  4012c3:	74 0b                	je     4012d0 <main+0x6a>
  4012c5:	48 89 de             	mov    %rbx,%rsi
  4012c8:	4c 89 e7             	mov    %r12,%rdi
  4012cb:	e8 9c 03 00 00       	callq  40166c <_ZdlPvS_>
  4012d0:	4c 89 e8             	mov    %r13,%rax
  4012d3:	48 89 c7             	mov    %rax,%rdi
  4012d6:	e8 35 fe ff ff       	callq  401110 <_Unwind_Resume@plt>
  4012db:	48 83 c4 10          	add    $0x10,%rsp
  4012df:	5b                   	pop    %rbx
  4012e0:	41 5c                	pop    %r12
  4012e2:	41 5d                	pop    %r13
  4012e4:	41 5e                	pop    %r14
  4012e6:	5d                   	pop    %rbp
  4012e7:	c3                   	retq   

00000000004012e8 <_ZNSt6threadC1IZ4mainEUlvE_JEvEEOT_DpOT0_>:
  4012e8:	55                   	push   %rbp
  4012e9:	48 89 e5             	mov    %rsp,%rbp
  4012ec:	41 55                	push   %r13
  4012ee:	41 54                	push   %r12
  4012f0:	53                   	push   %rbx
  4012f1:	48 83 ec 28          	sub    $0x28,%rsp
  4012f5:	48 89 7d c8          	mov    %rdi,-0x38(%rbp)
  4012f9:	48 89 75 c0          	mov    %rsi,-0x40(%rbp)
  4012fd:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
  401301:	48 89 c7             	mov    %rax,%rdi
  401304:	e8 73 03 00 00       	callq  40167c <_ZNSt6thread2idC1Ev>
  401309:	48 c7 45 d8 60 10 40 	movq   $0x401060,-0x28(%rbp)
  401310:	00 
  401311:	bf 10 00 00 00       	mov    $0x10,%edi
  401316:	e8 75 fd ff ff       	callq  401090 <_Znwm@plt>
  40131b:	48 89 c3             	mov    %rax,%rbx
  40131e:	41 bd 01 00 00 00    	mov    $0x1,%r13d
  401324:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
  401328:	48 89 c7             	mov    %rax,%rdi
  40132b:	e8 81 00 00 00       	callq  4013b1 <_ZSt7forwardIZ4mainEUlvE_EOT_RNSt16remove_referenceIS1_E4typeE>
  401330:	48 89 c6             	mov    %rax,%rsi
  401333:	48 89 df             	mov    %rbx,%rdi
  401336:	e8 85 00 00 00       	callq  4013c0 <_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZ4mainEUlvE_EEEEEC1IJS3_EEEDpOT_>
  40133b:	41 bd 00 00 00 00    	mov    $0x0,%r13d
  401341:	48 8d 45 d0          	lea    -0x30(%rbp),%rax
  401345:	48 89 de             	mov    %rbx,%rsi
  401348:	48 89 c7             	mov    %rax,%rdi
  40134b:	e8 f4 03 00 00       	callq  401744 <_ZNSt10unique_ptrINSt6thread6_StateESt14default_deleteIS1_EEC1IS3_vEEPS1_>
  401350:	48 8b 55 d8          	mov    -0x28(%rbp),%rdx
  401354:	48 8d 4d d0          	lea    -0x30(%rbp),%rcx
  401358:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
  40135c:	48 89 ce             	mov    %rcx,%rsi
  40135f:	48 89 c7             	mov    %rax,%rdi
  401362:	e8 19 fd ff ff       	callq  401080 <_ZNSt6thread15_M_start_threadESt10unique_ptrINS_6_StateESt14default_deleteIS1_EEPFvvE@plt>
  401367:	48 8d 45 d0          	lea    -0x30(%rbp),%rax
  40136b:	48 89 c7             	mov    %rax,%rdi
  40136e:	e8 11 04 00 00       	callq  401784 <_ZNSt10unique_ptrINSt6thread6_StateESt14default_deleteIS1_EED1Ev>
  401373:	eb 31                	jmp    4013a6 <_ZNSt6threadC1IZ4mainEUlvE_JEvEEOT_DpOT0_+0xbe>
  401375:	49 89 c4             	mov    %rax,%r12
  401378:	48 8d 45 d0          	lea    -0x30(%rbp),%rax
  40137c:	48 89 c7             	mov    %rax,%rdi
  40137f:	e8 00 04 00 00       	callq  401784 <_ZNSt10unique_ptrINSt6thread6_StateESt14default_deleteIS1_EED1Ev>
  401384:	eb 03                	jmp    401389 <_ZNSt6threadC1IZ4mainEUlvE_JEvEEOT_DpOT0_+0xa1>
  401386:	49 89 c4             	mov    %rax,%r12
  401389:	45 84 ed             	test   %r13b,%r13b
  40138c:	74 0d                	je     40139b <_ZNSt6threadC1IZ4mainEUlvE_JEvEEOT_DpOT0_+0xb3>
  40138e:	be 10 00 00 00       	mov    $0x10,%esi
  401393:	48 89 df             	mov    %rbx,%rdi
  401396:	e8 05 fd ff ff       	callq  4010a0 <_ZdlPvm@plt>
  40139b:	4c 89 e0             	mov    %r12,%rax
  40139e:	48 89 c7             	mov    %rax,%rdi
  4013a1:	e8 6a fd ff ff       	callq  401110 <_Unwind_Resume@plt>
  4013a6:	48 83 c4 28          	add    $0x28,%rsp
  4013aa:	5b                   	pop    %rbx
  4013ab:	41 5c                	pop    %r12
  4013ad:	41 5d                	pop    %r13
  4013af:	5d                   	pop    %rbp
  4013b0:	c3                   	retq   

00000000004013b1 <_ZSt7forwardIZ4mainEUlvE_EOT_RNSt16remove_referenceIS1_E4typeE>:
  4013b1:	55                   	push   %rbp
  4013b2:	48 89 e5             	mov    %rsp,%rbp
  4013b5:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4013b9:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4013bd:	5d                   	pop    %rbp
  4013be:	c3                   	retq   
  4013bf:	90                   	nop

00000000004013c0 <_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZ4mainEUlvE_EEEEEC1IJS3_EEEDpOT_>:
  4013c0:	55                   	push   %rbp
  4013c1:	48 89 e5             	mov    %rsp,%rbp
  4013c4:	53                   	push   %rbx
  4013c5:	48 83 ec 18          	sub    $0x18,%rsp
  4013c9:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  4013cd:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
  4013d1:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4013d5:	48 89 c7             	mov    %rax,%rdi
  4013d8:	e8 29 03 00 00       	callq  401706 <_ZNSt6thread6_StateC1Ev>
  4013dd:	ba 50 20 40 00       	mov    $0x402050,%edx
  4013e2:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4013e6:	48 89 10             	mov    %rdx,(%rax)
  4013e9:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4013ed:	48 8d 58 08          	lea    0x8(%rax),%rbx
  4013f1:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  4013f5:	48 89 c7             	mov    %rax,%rdi
  4013f8:	e8 b4 ff ff ff       	callq  4013b1 <_ZSt7forwardIZ4mainEUlvE_EOT_RNSt16remove_referenceIS1_E4typeE>
  4013fd:	48 89 c6             	mov    %rax,%rsi
  401400:	48 89 df             	mov    %rbx,%rdi
  401403:	e8 08 00 00 00       	callq  401410 <_ZNSt5tupleIJZ4mainEUlvE_EEC1IJS0_ELb1ELb1EEEDpOT_>
  401408:	90                   	nop
  401409:	48 8b 5d f8          	mov    -0x8(%rbp),%rbx
  40140d:	c9                   	leaveq 
  40140e:	c3                   	retq   
  40140f:	90                   	nop

0000000000401410 <_ZNSt5tupleIJZ4mainEUlvE_EEC1IJS0_ELb1ELb1EEEDpOT_>:
  401410:	55                   	push   %rbp
  401411:	48 89 e5             	mov    %rsp,%rbp
  401414:	48 83 ec 10          	sub    $0x10,%rsp
  401418:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40141c:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  401420:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  401424:	48 89 c7             	mov    %rax,%rdi
  401427:	e8 85 ff ff ff       	callq  4013b1 <_ZSt7forwardIZ4mainEUlvE_EOT_RNSt16remove_referenceIS1_E4typeE>
  40142c:	48 89 c2             	mov    %rax,%rdx
  40142f:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401433:	48 89 d6             	mov    %rdx,%rsi
  401436:	48 89 c7             	mov    %rax,%rdi
  401439:	e8 04 00 00 00       	callq  401442 <_ZNSt11_Tuple_implILm0EJZ4mainEUlvE_EEC2IS0_EEOT_>
  40143e:	90                   	nop
  40143f:	c9                   	leaveq 
  401440:	c3                   	retq   
  401441:	90                   	nop

0000000000401442 <_ZNSt11_Tuple_implILm0EJZ4mainEUlvE_EEC2IS0_EEOT_>:
  401442:	55                   	push   %rbp
  401443:	48 89 e5             	mov    %rsp,%rbp
  401446:	48 83 ec 10          	sub    $0x10,%rsp
  40144a:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40144e:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  401452:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  401456:	48 89 c7             	mov    %rax,%rdi
  401459:	e8 53 ff ff ff       	callq  4013b1 <_ZSt7forwardIZ4mainEUlvE_EOT_RNSt16remove_referenceIS1_E4typeE>
  40145e:	48 89 c2             	mov    %rax,%rdx
  401461:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401465:	48 89 d6             	mov    %rdx,%rsi
  401468:	48 89 c7             	mov    %rax,%rdi
  40146b:	e8 04 00 00 00       	callq  401474 <_ZNSt10_Head_baseILm0EZ4mainEUlvE_Lb1EEC2IS0_EEOT_>
  401470:	90                   	nop
  401471:	c9                   	leaveq 
  401472:	c3                   	retq   
  401473:	90                   	nop

0000000000401474 <_ZNSt10_Head_baseILm0EZ4mainEUlvE_Lb1EEC2IS0_EEOT_>:
  401474:	55                   	push   %rbp
  401475:	48 89 e5             	mov    %rsp,%rbp
  401478:	48 83 ec 10          	sub    $0x10,%rsp
  40147c:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401480:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  401484:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  401488:	48 89 c7             	mov    %rax,%rdi
  40148b:	e8 21 ff ff ff       	callq  4013b1 <_ZSt7forwardIZ4mainEUlvE_EOT_RNSt16remove_referenceIS1_E4typeE>
  401490:	90                   	nop
  401491:	c9                   	leaveq 
  401492:	c3                   	retq   
  401493:	90                   	nop

0000000000401494 <_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZ4mainEUlvE_EEEEED1Ev>:
  401494:	55                   	push   %rbp
  401495:	48 89 e5             	mov    %rsp,%rbp
  401498:	48 83 ec 10          	sub    $0x10,%rsp
  40149c:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4014a0:	ba 50 20 40 00       	mov    $0x402050,%edx
  4014a5:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4014a9:	48 89 10             	mov    %rdx,(%rax)
  4014ac:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4014b0:	48 89 c7             	mov    %rax,%rdi
  4014b3:	e8 88 fb ff ff       	callq  401040 <_ZNSt6thread6_StateD2Ev@plt>
  4014b8:	90                   	nop
  4014b9:	c9                   	leaveq 
  4014ba:	c3                   	retq   
  4014bb:	90                   	nop

00000000004014bc <_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZ4mainEUlvE_EEEEED0Ev>:
  4014bc:	55                   	push   %rbp
  4014bd:	48 89 e5             	mov    %rsp,%rbp
  4014c0:	48 83 ec 10          	sub    $0x10,%rsp
  4014c4:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4014c8:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4014cc:	48 89 c7             	mov    %rax,%rdi
  4014cf:	e8 c0 ff ff ff       	callq  401494 <_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZ4mainEUlvE_EEEEED1Ev>
  4014d4:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4014d8:	be 10 00 00 00       	mov    $0x10,%esi
  4014dd:	48 89 c7             	mov    %rax,%rdi
  4014e0:	e8 bb fb ff ff       	callq  4010a0 <_ZdlPvm@plt>
  4014e5:	c9                   	leaveq 
  4014e6:	c3                   	retq   

00000000004014e7 <_Z41__static_initialization_and_destruction_0ii>:
  4014e7:	55                   	push   %rbp
  4014e8:	48 89 e5             	mov    %rsp,%rbp
  4014eb:	48 83 ec 10          	sub    $0x10,%rsp
  4014ef:	89 7d fc             	mov    %edi,-0x4(%rbp)
  4014f2:	89 75 f8             	mov    %esi,-0x8(%rbp)
  4014f5:	83 7d fc 01          	cmpl   $0x1,-0x4(%rbp)
  4014f9:	75 27                	jne    401522 <_Z41__static_initialization_and_destruction_0ii+0x3b>
  4014fb:	81 7d f8 ff ff 00 00 	cmpl   $0xffff,-0x8(%rbp)
  401502:	75 1e                	jne    401522 <_Z41__static_initialization_and_destruction_0ii+0x3b>
  401504:	bf d1 41 40 00       	mov    $0x4041d1,%edi
  401509:	e8 d2 fb ff ff       	callq  4010e0 <_ZNSt8ios_base4InitC1Ev@plt>
  40150e:	ba a0 40 40 00       	mov    $0x4040a0,%edx
  401513:	be d1 41 40 00       	mov    $0x4041d1,%esi
  401518:	bf 20 11 40 00       	mov    $0x401120,%edi
  40151d:	e8 4e fb ff ff       	callq  401070 <__cxa_atexit@plt>
  401522:	90                   	nop
  401523:	c9                   	leaveq 
  401524:	c3                   	retq   
  401525:	90                   	nop

0000000000401526 <_ZNSt6thread11_State_implINS_8_InvokerISt5tupleIJZ4mainEUlvE_EEEEE6_M_runEv>:
  401526:	55                   	push   %rbp
  401527:	48 89 e5             	mov    %rsp,%rbp
  40152a:	48 83 ec 10          	sub    $0x10,%rsp
  40152e:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401532:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401536:	48 83 c0 08          	add    $0x8,%rax
  40153a:	48 89 c7             	mov    %rax,%rdi
  40153d:	e8 04 00 00 00       	callq  401546 <_ZNSt6thread8_InvokerISt5tupleIJZ4mainEUlvE_EEEclEv>
  401542:	90                   	nop
  401543:	c9                   	leaveq 
  401544:	c3                   	retq   
  401545:	90                   	nop

0000000000401546 <_ZNSt6thread8_InvokerISt5tupleIJZ4mainEUlvE_EEEclEv>:
  401546:	55                   	push   %rbp
  401547:	48 89 e5             	mov    %rsp,%rbp
  40154a:	48 83 ec 10          	sub    $0x10,%rsp
  40154e:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401552:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401556:	48 89 c7             	mov    %rax,%rdi
  401559:	e8 04 00 00 00       	callq  401562 <_ZNSt6thread8_InvokerISt5tupleIJZ4mainEUlvE_EEE9_M_invokeIJLm0EEEEvSt12_Index_tupleIJXspT_EEE>
  40155e:	90                   	nop
  40155f:	c9                   	leaveq 
  401560:	c3                   	retq   
  401561:	90                   	nop

0000000000401562 <_ZNSt6thread8_InvokerISt5tupleIJZ4mainEUlvE_EEE9_M_invokeIJLm0EEEEvSt12_Index_tupleIJXspT_EEE>:
  401562:	55                   	push   %rbp
  401563:	48 89 e5             	mov    %rsp,%rbp
  401566:	48 83 ec 10          	sub    $0x10,%rsp
  40156a:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40156e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401572:	48 89 c7             	mov    %rax,%rdi
  401575:	e8 13 00 00 00       	callq  40158d <_ZSt4moveIRSt5tupleIJZ4mainEUlvE_EEEONSt16remove_referenceIT_E4typeEOS5_>
  40157a:	48 89 c7             	mov    %rax,%rdi
  40157d:	e8 19 00 00 00       	callq  40159b <_ZSt3getILm0EJZ4mainEUlvE_EEONSt13tuple_elementIXT_ESt5tupleIJDpT0_EEE4typeEOS5_>
  401582:	48 89 c7             	mov    %rax,%rdi
  401585:	e8 33 00 00 00       	callq  4015bd <_ZSt8__invokeIZ4mainEUlvE_JEENSt15__invoke_resultIT_JDpT0_EE4typeEOS2_DpOS3_>
  40158a:	90                   	nop
  40158b:	c9                   	leaveq 
  40158c:	c3                   	retq   

000000000040158d <_ZSt4moveIRSt5tupleIJZ4mainEUlvE_EEEONSt16remove_referenceIT_E4typeEOS5_>:
  40158d:	55                   	push   %rbp
  40158e:	48 89 e5             	mov    %rsp,%rbp
  401591:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401595:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401599:	5d                   	pop    %rbp
  40159a:	c3                   	retq   

000000000040159b <_ZSt3getILm0EJZ4mainEUlvE_EEONSt13tuple_elementIXT_ESt5tupleIJDpT0_EEE4typeEOS5_>:
  40159b:	55                   	push   %rbp
  40159c:	48 89 e5             	mov    %rsp,%rbp
  40159f:	48 83 ec 10          	sub    $0x10,%rsp
  4015a3:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4015a7:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4015ab:	48 89 c7             	mov    %rax,%rdi
  4015ae:	e8 2d 00 00 00       	callq  4015e0 <_ZSt12__get_helperILm0EZ4mainEUlvE_JEERT0_RSt11_Tuple_implIXT_EJS1_DpT1_EE>
  4015b3:	48 89 c7             	mov    %rax,%rdi
  4015b6:	e8 f6 fd ff ff       	callq  4013b1 <_ZSt7forwardIZ4mainEUlvE_EOT_RNSt16remove_referenceIS1_E4typeE>
  4015bb:	c9                   	leaveq 
  4015bc:	c3                   	retq   

00000000004015bd <_ZSt8__invokeIZ4mainEUlvE_JEENSt15__invoke_resultIT_JDpT0_EE4typeEOS2_DpOS3_>:
  4015bd:	55                   	push   %rbp
  4015be:	48 89 e5             	mov    %rsp,%rbp
  4015c1:	48 83 ec 10          	sub    $0x10,%rsp
  4015c5:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4015c9:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4015cd:	48 89 c7             	mov    %rax,%rdi
  4015d0:	e8 dc fd ff ff       	callq  4013b1 <_ZSt7forwardIZ4mainEUlvE_EOT_RNSt16remove_referenceIS1_E4typeE>
  4015d5:	48 89 c7             	mov    %rax,%rdi
  4015d8:	e8 1d 00 00 00       	callq  4015fa <_ZSt13__invoke_implIvZ4mainEUlvE_JEET_St14__invoke_otherOT0_DpOT1_>
  4015dd:	90                   	nop
  4015de:	c9                   	leaveq 
  4015df:	c3                   	retq   

00000000004015e0 <_ZSt12__get_helperILm0EZ4mainEUlvE_JEERT0_RSt11_Tuple_implIXT_EJS1_DpT1_EE>:
  4015e0:	55                   	push   %rbp
  4015e1:	48 89 e5             	mov    %rsp,%rbp
  4015e4:	48 83 ec 10          	sub    $0x10,%rsp
  4015e8:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4015ec:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4015f0:	48 89 c7             	mov    %rax,%rdi
  4015f3:	e8 25 00 00 00       	callq  40161d <_ZNSt11_Tuple_implILm0EJZ4mainEUlvE_EE7_M_headERS1_>
  4015f8:	c9                   	leaveq 
  4015f9:	c3                   	retq   

00000000004015fa <_ZSt13__invoke_implIvZ4mainEUlvE_JEET_St14__invoke_otherOT0_DpOT1_>:
  4015fa:	55                   	push   %rbp
  4015fb:	48 89 e5             	mov    %rsp,%rbp
  4015fe:	48 83 ec 10          	sub    $0x10,%rsp
  401602:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401606:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40160a:	48 89 c7             	mov    %rax,%rdi
  40160d:	e8 9f fd ff ff       	callq  4013b1 <_ZSt7forwardIZ4mainEUlvE_EOT_RNSt16remove_referenceIS1_E4typeE>
  401612:	48 89 c7             	mov    %rax,%rdi
  401615:	e8 fc fb ff ff       	callq  401216 <_ZZ4mainENKUlvE_clEv>
  40161a:	90                   	nop
  40161b:	c9                   	leaveq 
  40161c:	c3                   	retq   

000000000040161d <_ZNSt11_Tuple_implILm0EJZ4mainEUlvE_EE7_M_headERS1_>:
  40161d:	55                   	push   %rbp
  40161e:	48 89 e5             	mov    %rsp,%rbp
  401621:	48 83 ec 10          	sub    $0x10,%rsp
  401625:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401629:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40162d:	48 89 c7             	mov    %rax,%rdi
  401630:	e8 02 00 00 00       	callq  401637 <_ZNSt10_Head_baseILm0EZ4mainEUlvE_Lb1EE7_M_headERS1_>
  401635:	c9                   	leaveq 
  401636:	c3                   	retq   

0000000000401637 <_ZNSt10_Head_baseILm0EZ4mainEUlvE_Lb1EE7_M_headERS1_>:
  401637:	55                   	push   %rbp
  401638:	48 89 e5             	mov    %rsp,%rbp
  40163b:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40163f:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401643:	5d                   	pop    %rbp
  401644:	c3                   	retq   

0000000000401645 <_GLOBAL__sub_I_main>:
  401645:	55                   	push   %rbp
  401646:	48 89 e5             	mov    %rsp,%rbp
  401649:	be ff ff 00 00       	mov    $0xffff,%esi
  40164e:	bf 01 00 00 00       	mov    $0x1,%edi
  401653:	e8 8f fe ff ff       	callq  4014e7 <_Z41__static_initialization_and_destruction_0ii>
  401658:	5d                   	pop    %rbp
  401659:	c3                   	retq   

000000000040165a <_ZnwmPv>:
  40165a:	55                   	push   %rbp
  40165b:	48 89 e5             	mov    %rsp,%rbp
  40165e:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401662:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  401666:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  40166a:	5d                   	pop    %rbp
  40166b:	c3                   	retq   

000000000040166c <_ZdlPvS_>:
  40166c:	55                   	push   %rbp
  40166d:	48 89 e5             	mov    %rsp,%rbp
  401670:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401674:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  401678:	90                   	nop
  401679:	5d                   	pop    %rbp
  40167a:	c3                   	retq   
  40167b:	90                   	nop

000000000040167c <_ZNSt6thread2idC1Ev>:
  40167c:	55                   	push   %rbp
  40167d:	48 89 e5             	mov    %rsp,%rbp
  401680:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401684:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401688:	48 c7 00 00 00 00 00 	movq   $0x0,(%rax)
  40168f:	90                   	nop
  401690:	5d                   	pop    %rbp
  401691:	c3                   	retq   

0000000000401692 <_ZNSt6threadD1Ev>:
  401692:	55                   	push   %rbp
  401693:	48 89 e5             	mov    %rsp,%rbp
  401696:	48 83 ec 10          	sub    $0x10,%rsp
  40169a:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40169e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4016a2:	48 89 c7             	mov    %rax,%rdi
  4016a5:	e8 0c 00 00 00       	callq  4016b6 <_ZNKSt6thread8joinableEv>
  4016aa:	84 c0                	test   %al,%al
  4016ac:	74 05                	je     4016b3 <_ZNSt6threadD1Ev+0x21>
  4016ae:	e8 7d f9 ff ff       	callq  401030 <_ZSt9terminatev@plt>
  4016b3:	90                   	nop
  4016b4:	c9                   	leaveq 
  4016b5:	c3                   	retq   

00000000004016b6 <_ZNKSt6thread8joinableEv>:
  4016b6:	55                   	push   %rbp
  4016b7:	48 89 e5             	mov    %rsp,%rbp
  4016ba:	48 83 ec 20          	sub    $0x20,%rsp
  4016be:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  4016c2:	48 8d 45 f8          	lea    -0x8(%rbp),%rax
  4016c6:	48 89 c7             	mov    %rax,%rdi
  4016c9:	e8 ae ff ff ff       	callq  40167c <_ZNSt6thread2idC1Ev>
  4016ce:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
  4016d2:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4016d6:	48 8b 00             	mov    (%rax),%rax
  4016d9:	48 89 d6             	mov    %rdx,%rsi
  4016dc:	48 89 c7             	mov    %rax,%rdi
  4016df:	e8 05 00 00 00       	callq  4016e9 <_ZSteqNSt6thread2idES0_>
  4016e4:	83 f0 01             	xor    $0x1,%eax
  4016e7:	c9                   	leaveq 
  4016e8:	c3                   	retq   

00000000004016e9 <_ZSteqNSt6thread2idES0_>:
  4016e9:	55                   	push   %rbp
  4016ea:	48 89 e5             	mov    %rsp,%rbp
  4016ed:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4016f1:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  4016f5:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
  4016f9:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  4016fd:	48 39 c2             	cmp    %rax,%rdx
  401700:	0f 94 c0             	sete   %al
  401703:	5d                   	pop    %rbp
  401704:	c3                   	retq   
  401705:	90                   	nop

0000000000401706 <_ZNSt6thread6_StateC1Ev>:
  401706:	55                   	push   %rbp
  401707:	48 89 e5             	mov    %rsp,%rbp
  40170a:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40170e:	ba 30 3d 40 00       	mov    $0x403d30,%edx
  401713:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401717:	48 89 10             	mov    %rdx,(%rax)
  40171a:	90                   	nop
  40171b:	5d                   	pop    %rbp
  40171c:	c3                   	retq   
  40171d:	90                   	nop

000000000040171e <_ZNSt15__uniq_ptr_dataINSt6thread6_StateESt14default_deleteIS1_ELb1ELb1EECI1St15__uniq_ptr_implIS1_S3_EEPS1_>:
  40171e:	55                   	push   %rbp
  40171f:	48 89 e5             	mov    %rsp,%rbp
  401722:	48 83 ec 10          	sub    $0x10,%rsp
  401726:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40172a:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  40172e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401732:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
  401736:	48 89 d6             	mov    %rdx,%rsi
  401739:	48 89 c7             	mov    %rax,%rdi
  40173c:	e8 a7 00 00 00       	callq  4017e8 <_ZNSt15__uniq_ptr_implINSt6thread6_StateESt14default_deleteIS1_EEC1EPS1_>
  401741:	90                   	nop
  401742:	c9                   	leaveq 
  401743:	c3                   	retq   

0000000000401744 <_ZNSt10unique_ptrINSt6thread6_StateESt14default_deleteIS1_EEC1IS3_vEEPS1_>:
  401744:	55                   	push   %rbp
  401745:	48 89 e5             	mov    %rsp,%rbp
  401748:	48 83 ec 10          	sub    $0x10,%rsp
  40174c:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401750:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  401754:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401758:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
  40175c:	48 89 d6             	mov    %rdx,%rsi
  40175f:	48 89 c7             	mov    %rax,%rdi
  401762:	e8 b7 ff ff ff       	callq  40171e <_ZNSt15__uniq_ptr_dataINSt6thread6_StateESt14default_deleteIS1_ELb1ELb1EECI1St15__uniq_ptr_implIS1_S3_EEPS1_>
  401767:	90                   	nop
  401768:	c9                   	leaveq 
  401769:	c3                   	retq   

000000000040176a <_ZNSt15__uniq_ptr_implINSt6thread6_StateESt14default_deleteIS1_EE6_M_ptrEv>:
  40176a:	55                   	push   %rbp
  40176b:	48 89 e5             	mov    %rsp,%rbp
  40176e:	48 83 ec 10          	sub    $0x10,%rsp
  401772:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401776:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40177a:	48 89 c7             	mov    %rax,%rdi
  40177d:	e8 9d 00 00 00       	callq  40181f <_ZSt3getILm0EJPNSt6thread6_StateESt14default_deleteIS1_EEERNSt13tuple_elementIXT_ESt5tupleIJDpT0_EEE4typeERS9_>
  401782:	c9                   	leaveq 
  401783:	c3                   	retq   

0000000000401784 <_ZNSt10unique_ptrINSt6thread6_StateESt14default_deleteIS1_EED1Ev>:
  401784:	55                   	push   %rbp
  401785:	48 89 e5             	mov    %rsp,%rbp
  401788:	53                   	push   %rbx
  401789:	48 83 ec 28          	sub    $0x28,%rsp
  40178d:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
  401791:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
  401795:	48 89 c7             	mov    %rax,%rdi
  401798:	e8 cd ff ff ff       	callq  40176a <_ZNSt15__uniq_ptr_implINSt6thread6_StateESt14default_deleteIS1_EE6_M_ptrEv>
  40179d:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
  4017a1:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4017a5:	48 8b 00             	mov    (%rax),%rax
  4017a8:	48 85 c0             	test   %rax,%rax
  4017ab:	74 29                	je     4017d6 <_ZNSt10unique_ptrINSt6thread6_StateESt14default_deleteIS1_EED1Ev+0x52>
  4017ad:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
  4017b1:	48 89 c7             	mov    %rax,%rdi
  4017b4:	e8 81 00 00 00       	callq  40183a <_ZNSt10unique_ptrINSt6thread6_StateESt14default_deleteIS1_EE11get_deleterEv>
  4017b9:	48 89 c3             	mov    %rax,%rbx
  4017bc:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4017c0:	48 89 c7             	mov    %rax,%rdi
  4017c3:	e8 8c 00 00 00       	callq  401854 <_ZSt4moveIRPNSt6thread6_StateEEONSt16remove_referenceIT_E4typeEOS5_>
  4017c8:	48 8b 00             	mov    (%rax),%rax
  4017cb:	48 89 c6             	mov    %rax,%rsi
  4017ce:	48 89 df             	mov    %rbx,%rdi
  4017d1:	e8 8c 00 00 00       	callq  401862 <_ZNKSt14default_deleteINSt6thread6_StateEEclEPS1_>
  4017d6:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4017da:	48 c7 00 00 00 00 00 	movq   $0x0,(%rax)
  4017e1:	90                   	nop
  4017e2:	48 8b 5d f8          	mov    -0x8(%rbp),%rbx
  4017e6:	c9                   	leaveq 
  4017e7:	c3                   	retq   

00000000004017e8 <_ZNSt15__uniq_ptr_implINSt6thread6_StateESt14default_deleteIS1_EEC1EPS1_>:
  4017e8:	55                   	push   %rbp
  4017e9:	48 89 e5             	mov    %rsp,%rbp
  4017ec:	53                   	push   %rbx
  4017ed:	48 83 ec 18          	sub    $0x18,%rsp
  4017f1:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
  4017f5:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
  4017f9:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  4017fd:	48 89 c7             	mov    %rax,%rdi
  401800:	e8 89 00 00 00       	callq  40188e <_ZNSt5tupleIJPNSt6thread6_StateESt14default_deleteIS1_EEEC1ILb1ELb1EEEv>
  401805:	48 8b 5d e0          	mov    -0x20(%rbp),%rbx
  401809:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
  40180d:	48 89 c7             	mov    %rax,%rdi
  401810:	e8 55 ff ff ff       	callq  40176a <_ZNSt15__uniq_ptr_implINSt6thread6_StateESt14default_deleteIS1_EE6_M_ptrEv>
  401815:	48 89 18             	mov    %rbx,(%rax)
  401818:	90                   	nop
  401819:	48 8b 5d f8          	mov    -0x8(%rbp),%rbx
  40181d:	c9                   	leaveq 
  40181e:	c3                   	retq   

000000000040181f <_ZSt3getILm0EJPNSt6thread6_StateESt14default_deleteIS1_EEERNSt13tuple_elementIXT_ESt5tupleIJDpT0_EEE4typeERS9_>:
  40181f:	55                   	push   %rbp
  401820:	48 89 e5             	mov    %rsp,%rbp
  401823:	48 83 ec 10          	sub    $0x10,%rsp
  401827:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40182b:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40182f:	48 89 c7             	mov    %rax,%rdi
  401832:	e8 72 00 00 00       	callq  4018a9 <_ZSt12__get_helperILm0EPNSt6thread6_StateEJSt14default_deleteIS1_EEERT0_RSt11_Tuple_implIXT_EJS5_DpT1_EE>
  401837:	c9                   	leaveq 
  401838:	c3                   	retq   
  401839:	90                   	nop

000000000040183a <_ZNSt10unique_ptrINSt6thread6_StateESt14default_deleteIS1_EE11get_deleterEv>:
  40183a:	55                   	push   %rbp
  40183b:	48 89 e5             	mov    %rsp,%rbp
  40183e:	48 83 ec 10          	sub    $0x10,%rsp
  401842:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401846:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40184a:	48 89 c7             	mov    %rax,%rdi
  40184d:	e8 72 00 00 00       	callq  4018c4 <_ZNSt15__uniq_ptr_implINSt6thread6_StateESt14default_deleteIS1_EE10_M_deleterEv>
  401852:	c9                   	leaveq 
  401853:	c3                   	retq   

0000000000401854 <_ZSt4moveIRPNSt6thread6_StateEEONSt16remove_referenceIT_E4typeEOS5_>:
  401854:	55                   	push   %rbp
  401855:	48 89 e5             	mov    %rsp,%rbp
  401858:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40185c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401860:	5d                   	pop    %rbp
  401861:	c3                   	retq   

0000000000401862 <_ZNKSt14default_deleteINSt6thread6_StateEEclEPS1_>:
  401862:	55                   	push   %rbp
  401863:	48 89 e5             	mov    %rsp,%rbp
  401866:	48 83 ec 10          	sub    $0x10,%rsp
  40186a:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40186e:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  401872:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  401876:	48 85 c0             	test   %rax,%rax
  401879:	74 0f                	je     40188a <_ZNKSt14default_deleteINSt6thread6_StateEEclEPS1_+0x28>
  40187b:	48 8b 10             	mov    (%rax),%rdx
  40187e:	48 83 c2 08          	add    $0x8,%rdx
  401882:	48 8b 12             	mov    (%rdx),%rdx
  401885:	48 89 c7             	mov    %rax,%rdi
  401888:	ff d2                	callq  *%rdx
  40188a:	90                   	nop
  40188b:	c9                   	leaveq 
  40188c:	c3                   	retq   
  40188d:	90                   	nop

000000000040188e <_ZNSt5tupleIJPNSt6thread6_StateESt14default_deleteIS1_EEEC1ILb1ELb1EEEv>:
  40188e:	55                   	push   %rbp
  40188f:	48 89 e5             	mov    %rsp,%rbp
  401892:	48 83 ec 10          	sub    $0x10,%rsp
  401896:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40189a:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40189e:	48 89 c7             	mov    %rax,%rdi
  4018a1:	e8 38 00 00 00       	callq  4018de <_ZNSt11_Tuple_implILm0EJPNSt6thread6_StateESt14default_deleteIS1_EEEC1Ev>
  4018a6:	90                   	nop
  4018a7:	c9                   	leaveq 
  4018a8:	c3                   	retq   

00000000004018a9 <_ZSt12__get_helperILm0EPNSt6thread6_StateEJSt14default_deleteIS1_EEERT0_RSt11_Tuple_implIXT_EJS5_DpT1_EE>:
  4018a9:	55                   	push   %rbp
  4018aa:	48 89 e5             	mov    %rsp,%rbp
  4018ad:	48 83 ec 10          	sub    $0x10,%rsp
  4018b1:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4018b5:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4018b9:	48 89 c7             	mov    %rax,%rdi
  4018bc:	e8 44 00 00 00       	callq  401905 <_ZNSt11_Tuple_implILm0EJPNSt6thread6_StateESt14default_deleteIS1_EEE7_M_headERS5_>
  4018c1:	c9                   	leaveq 
  4018c2:	c3                   	retq   
  4018c3:	90                   	nop

00000000004018c4 <_ZNSt15__uniq_ptr_implINSt6thread6_StateESt14default_deleteIS1_EE10_M_deleterEv>:
  4018c4:	55                   	push   %rbp
  4018c5:	48 89 e5             	mov    %rsp,%rbp
  4018c8:	48 83 ec 10          	sub    $0x10,%rsp
  4018cc:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4018d0:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4018d4:	48 89 c7             	mov    %rax,%rdi
  4018d7:	e8 43 00 00 00       	callq  40191f <_ZSt3getILm1EJPNSt6thread6_StateESt14default_deleteIS1_EEERNSt13tuple_elementIXT_ESt5tupleIJDpT0_EEE4typeERS9_>
  4018dc:	c9                   	leaveq 
  4018dd:	c3                   	retq   

00000000004018de <_ZNSt11_Tuple_implILm0EJPNSt6thread6_StateESt14default_deleteIS1_EEEC1Ev>:
  4018de:	55                   	push   %rbp
  4018df:	48 89 e5             	mov    %rsp,%rbp
  4018e2:	48 83 ec 10          	sub    $0x10,%rsp
  4018e6:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4018ea:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4018ee:	48 89 c7             	mov    %rax,%rdi
  4018f1:	e8 44 00 00 00       	callq  40193a <_ZNSt11_Tuple_implILm1EJSt14default_deleteINSt6thread6_StateEEEEC1Ev>
  4018f6:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4018fa:	48 89 c7             	mov    %rax,%rdi
  4018fd:	e8 54 00 00 00       	callq  401956 <_ZNSt10_Head_baseILm0EPNSt6thread6_StateELb0EEC1Ev>
  401902:	90                   	nop
  401903:	c9                   	leaveq 
  401904:	c3                   	retq   

0000000000401905 <_ZNSt11_Tuple_implILm0EJPNSt6thread6_StateESt14default_deleteIS1_EEE7_M_headERS5_>:
  401905:	55                   	push   %rbp
  401906:	48 89 e5             	mov    %rsp,%rbp
  401909:	48 83 ec 10          	sub    $0x10,%rsp
  40190d:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401911:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401915:	48 89 c7             	mov    %rax,%rdi
  401918:	e8 4f 00 00 00       	callq  40196c <_ZNSt10_Head_baseILm0EPNSt6thread6_StateELb0EE7_M_headERS3_>
  40191d:	c9                   	leaveq 
  40191e:	c3                   	retq   

000000000040191f <_ZSt3getILm1EJPNSt6thread6_StateESt14default_deleteIS1_EEERNSt13tuple_elementIXT_ESt5tupleIJDpT0_EEE4typeERS9_>:
  40191f:	55                   	push   %rbp
  401920:	48 89 e5             	mov    %rsp,%rbp
  401923:	48 83 ec 10          	sub    $0x10,%rsp
  401927:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40192b:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40192f:	48 89 c7             	mov    %rax,%rdi
  401932:	e8 43 00 00 00       	callq  40197a <_ZSt12__get_helperILm1ESt14default_deleteINSt6thread6_StateEEJEERT0_RSt11_Tuple_implIXT_EJS4_DpT1_EE>
  401937:	c9                   	leaveq 
  401938:	c3                   	retq   
  401939:	90                   	nop

000000000040193a <_ZNSt11_Tuple_implILm1EJSt14default_deleteINSt6thread6_StateEEEEC1Ev>:
  40193a:	55                   	push   %rbp
  40193b:	48 89 e5             	mov    %rsp,%rbp
  40193e:	48 83 ec 10          	sub    $0x10,%rsp
  401942:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401946:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40194a:	48 89 c7             	mov    %rax,%rdi
  40194d:	e8 42 00 00 00       	callq  401994 <_ZNSt10_Head_baseILm1ESt14default_deleteINSt6thread6_StateEELb1EEC1Ev>
  401952:	90                   	nop
  401953:	c9                   	leaveq 
  401954:	c3                   	retq   
  401955:	90                   	nop

0000000000401956 <_ZNSt10_Head_baseILm0EPNSt6thread6_StateELb0EEC1Ev>:
  401956:	55                   	push   %rbp
  401957:	48 89 e5             	mov    %rsp,%rbp
  40195a:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40195e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401962:	48 c7 00 00 00 00 00 	movq   $0x0,(%rax)
  401969:	90                   	nop
  40196a:	5d                   	pop    %rbp
  40196b:	c3                   	retq   

000000000040196c <_ZNSt10_Head_baseILm0EPNSt6thread6_StateELb0EE7_M_headERS3_>:
  40196c:	55                   	push   %rbp
  40196d:	48 89 e5             	mov    %rsp,%rbp
  401970:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401974:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401978:	5d                   	pop    %rbp
  401979:	c3                   	retq   

000000000040197a <_ZSt12__get_helperILm1ESt14default_deleteINSt6thread6_StateEEJEERT0_RSt11_Tuple_implIXT_EJS4_DpT1_EE>:
  40197a:	55                   	push   %rbp
  40197b:	48 89 e5             	mov    %rsp,%rbp
  40197e:	48 83 ec 10          	sub    $0x10,%rsp
  401982:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  401986:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  40198a:	48 89 c7             	mov    %rax,%rdi
  40198d:	e8 0d 00 00 00       	callq  40199f <_ZNSt11_Tuple_implILm1EJSt14default_deleteINSt6thread6_StateEEEE7_M_headERS4_>
  401992:	c9                   	leaveq 
  401993:	c3                   	retq   

0000000000401994 <_ZNSt10_Head_baseILm1ESt14default_deleteINSt6thread6_StateEELb1EEC1Ev>:
  401994:	55                   	push   %rbp
  401995:	48 89 e5             	mov    %rsp,%rbp
  401998:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40199c:	90                   	nop
  40199d:	5d                   	pop    %rbp
  40199e:	c3                   	retq   

000000000040199f <_ZNSt11_Tuple_implILm1EJSt14default_deleteINSt6thread6_StateEEEE7_M_headERS4_>:
  40199f:	55                   	push   %rbp
  4019a0:	48 89 e5             	mov    %rsp,%rbp
  4019a3:	48 83 ec 10          	sub    $0x10,%rsp
  4019a7:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4019ab:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4019af:	48 89 c7             	mov    %rax,%rdi
  4019b2:	e8 02 00 00 00       	callq  4019b9 <_ZNSt10_Head_baseILm1ESt14default_deleteINSt6thread6_StateEELb1EE7_M_headERS4_>
  4019b7:	c9                   	leaveq 
  4019b8:	c3                   	retq   

00000000004019b9 <_ZNSt10_Head_baseILm1ESt14default_deleteINSt6thread6_StateEELb1EE7_M_headERS4_>:
  4019b9:	55                   	push   %rbp
  4019ba:	48 89 e5             	mov    %rsp,%rbp
  4019bd:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4019c1:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4019c5:	5d                   	pop    %rbp
  4019c6:	c3                   	retq   
  4019c7:	66 0f 1f 84 00 00 00 	nopw   0x0(%rax,%rax,1)
  4019ce:	00 00 

00000000004019d0 <__libc_csu_init>:
  4019d0:	f3 0f 1e fa          	endbr64 
  4019d4:	41 57                	push   %r15
  4019d6:	4c 8d 3d 0b 23 00 00 	lea    0x230b(%rip),%r15        # 403ce8 <__frame_dummy_init_array_entry>
  4019dd:	41 56                	push   %r14
  4019df:	49 89 d6             	mov    %rdx,%r14
  4019e2:	41 55                	push   %r13
  4019e4:	49 89 f5             	mov    %rsi,%r13
  4019e7:	41 54                	push   %r12
  4019e9:	41 89 fc             	mov    %edi,%r12d
  4019ec:	55                   	push   %rbp
  4019ed:	48 8d 2d 04 23 00 00 	lea    0x2304(%rip),%rbp        # 403cf8 <__do_global_dtors_aux_fini_array_entry>
  4019f4:	53                   	push   %rbx
  4019f5:	4c 29 fd             	sub    %r15,%rbp
  4019f8:	48 83 ec 08          	sub    $0x8,%rsp
  4019fc:	e8 ff f5 ff ff       	callq  401000 <_init>
  401a01:	48 c1 fd 03          	sar    $0x3,%rbp
  401a05:	74 1f                	je     401a26 <__libc_csu_init+0x56>
  401a07:	31 db                	xor    %ebx,%ebx
  401a09:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
  401a10:	4c 89 f2             	mov    %r14,%rdx
  401a13:	4c 89 ee             	mov    %r13,%rsi
  401a16:	44 89 e7             	mov    %r12d,%edi
  401a19:	41 ff 14 df          	callq  *(%r15,%rbx,8)
  401a1d:	48 83 c3 01          	add    $0x1,%rbx
  401a21:	48 39 dd             	cmp    %rbx,%rbp
  401a24:	75 ea                	jne    401a10 <__libc_csu_init+0x40>
  401a26:	48 83 c4 08          	add    $0x8,%rsp
  401a2a:	5b                   	pop    %rbx
  401a2b:	5d                   	pop    %rbp
  401a2c:	41 5c                	pop    %r12
  401a2e:	41 5d                	pop    %r13
  401a30:	41 5e                	pop    %r14
  401a32:	41 5f                	pop    %r15
  401a34:	c3                   	retq   
  401a35:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
  401a3c:	00 00 00 00 

0000000000401a40 <__libc_csu_fini>:
  401a40:	f3 0f 1e fa          	endbr64 
  401a44:	c3                   	retq   

Disassembly of section .fini:

0000000000401a48 <_fini>:
  401a48:	f3 0f 1e fa          	endbr64 
  401a4c:	48 83 ec 08          	sub    $0x8,%rsp
  401a50:	48 83 c4 08          	add    $0x8,%rsp
  401a54:	c3                   	retq   
