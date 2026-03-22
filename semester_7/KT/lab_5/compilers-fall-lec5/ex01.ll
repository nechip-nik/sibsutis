; ModuleID = './ex01.cpp'
source_filename = "./ex01.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

module asm ".globl _ZSt21ios_base_library_initv"

%"class.std::basic_ostream" = type { ptr, %"class.std::basic_ios" }
%"class.std::basic_ios" = type { %"class.std::ios_base", ptr, i8, i8, ptr, ptr, ptr, ptr }
%"class.std::ios_base" = type { ptr, i64, i64, i32, i32, i32, ptr, %"struct.std::ios_base::_Words", [8 x %"struct.std::ios_base::_Words"], i32, ptr, %"class.std::locale" }
%"struct.std::ios_base::_Words" = type { ptr, i64 }
%"class.std::locale" = type { ptr }
%class.test = type { i8 }

$_ZN4testC2Ev = comdat any

$_ZN4testD2Ev = comdat any

$__clang_call_terminate = comdat any

@_ZSt4cout = external global %"class.std::basic_ostream", align 8
@_ZTIi = external constant ptr
@_ZTId = external constant ptr
@.str = private unnamed_addr constant [18 x i8] c"Hello constructor\00", align 1, !dbg !0
@.str.1 = private unnamed_addr constant [17 x i8] c"Hello destructor\00", align 1, !dbg !8

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local noundef i32 @main(i32 noundef %0, ptr noundef %1) #0 !dbg !803 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %0, ptr %3, align 4
  call void @llvm.dbg.declare(metadata ptr %3, metadata !807, metadata !DIExpression()), !dbg !808
  store ptr %1, ptr %4, align 8
  call void @llvm.dbg.declare(metadata ptr %4, metadata !809, metadata !DIExpression()), !dbg !810
  %5 = load i32, ptr %3, align 4, !dbg !811
  %6 = call noundef i32 @_Z3fooi(i32 noundef %5), !dbg !812
  %7 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEi(ptr noundef nonnull align 8 dereferenceable(8) @_ZSt4cout, i32 noundef %6), !dbg !813
  ret i32 0, !dbg !814
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare noundef nonnull align 8 dereferenceable(8) ptr @_ZNSolsEi(ptr noundef nonnull align 8 dereferenceable(8), i32 noundef) #2

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local noundef i32 @_Z3fooi(i32 noundef %0) #3 personality ptr @__gxx_personality_v0 !dbg !815 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  call void @llvm.dbg.declare(metadata ptr %2, metadata !816, metadata !DIExpression()), !dbg !817
  call void @llvm.dbg.declare(metadata ptr %3, metadata !818, metadata !DIExpression()), !dbg !819
  %7 = load i32, ptr %2, align 4, !dbg !820
  %8 = invoke noundef i32 @_Z3bari(i32 noundef %7)
          to label %9 unwind label %10, !dbg !822

9:                                                ; preds = %1
  store i32 %8, ptr %3, align 4, !dbg !823
  br label %23, !dbg !824

10:                                               ; preds = %1
  %11 = landingpad { ptr, i32 }
          catch ptr @_ZTIi, !dbg !825
  %12 = extractvalue { ptr, i32 } %11, 0, !dbg !825
  store ptr %12, ptr %4, align 8, !dbg !825
  %13 = extractvalue { ptr, i32 } %11, 1, !dbg !825
  store i32 %13, ptr %5, align 4, !dbg !825
  br label %14, !dbg !825

14:                                               ; preds = %10
  %15 = load i32, ptr %5, align 4, !dbg !824
  %16 = call i32 @llvm.eh.typeid.for(ptr @_ZTIi) #7, !dbg !824
  %17 = icmp eq i32 %15, %16, !dbg !824
  br i1 %17, label %18, label %25, !dbg !824

18:                                               ; preds = %14
  call void @llvm.dbg.declare(metadata ptr %6, metadata !826, metadata !DIExpression()), !dbg !827
  %19 = load ptr, ptr %4, align 8, !dbg !824
  %20 = call ptr @__cxa_begin_catch(ptr %19) #7, !dbg !824
  %21 = load i32, ptr %20, align 4, !dbg !824
  store i32 %21, ptr %6, align 4, !dbg !824
  %22 = load i32, ptr %6, align 4, !dbg !828
  store i32 %22, ptr %3, align 4, !dbg !830
  call void @__cxa_end_catch() #7, !dbg !831
  br label %23, !dbg !831

23:                                               ; preds = %18, %9
  %24 = load i32, ptr %3, align 4, !dbg !832
  ret i32 %24, !dbg !833

25:                                               ; preds = %14
  %26 = load ptr, ptr %4, align 8, !dbg !824
  %27 = load i32, ptr %5, align 4, !dbg !824
  %28 = insertvalue { ptr, i32 } poison, ptr %26, 0, !dbg !824
  %29 = insertvalue { ptr, i32 } %28, i32 %27, 1, !dbg !824
  resume { ptr, i32 } %29, !dbg !824
}

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local noundef i32 @_Z3bari(i32 noundef %0) #3 personality ptr @__gxx_personality_v0 !dbg !834 {
  %2 = alloca i32, align 4
  %3 = alloca %class.test, align 1
  %4 = alloca %class.test, align 1
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  call void @llvm.dbg.declare(metadata ptr %2, metadata !835, metadata !DIExpression()), !dbg !836
  call void @llvm.dbg.declare(metadata ptr %3, metadata !837, metadata !DIExpression()), !dbg !838
  call void @_ZN4testC2Ev(ptr noundef nonnull align 1 dereferenceable(1) %3), !dbg !838
  call void @llvm.dbg.declare(metadata ptr %4, metadata !839, metadata !DIExpression()), !dbg !840
  invoke void @_ZN4testC2Ev(ptr noundef nonnull align 1 dereferenceable(1) %4)
          to label %7 unwind label %12, !dbg !840

7:                                                ; preds = %1
  %8 = load i32, ptr %2, align 4, !dbg !841
  %9 = icmp eq i32 %8, 1, !dbg !843
  br i1 %9, label %10, label %20, !dbg !844

10:                                               ; preds = %7
  %11 = call ptr @__cxa_allocate_exception(i64 4) #7, !dbg !845
  store i32 1, ptr %11, align 16, !dbg !845
  invoke void @__cxa_throw(ptr %11, ptr @_ZTIi, ptr null) #8
          to label %33 unwind label %16, !dbg !845

12:                                               ; preds = %1
  %13 = landingpad { ptr, i32 }
          cleanup, !dbg !847
  %14 = extractvalue { ptr, i32 } %13, 0, !dbg !847
  store ptr %14, ptr %5, align 8, !dbg !847
  %15 = extractvalue { ptr, i32 } %13, 1, !dbg !847
  store i32 %15, ptr %6, align 4, !dbg !847
  br label %27, !dbg !847

16:                                               ; preds = %23, %10
  %17 = landingpad { ptr, i32 }
          cleanup, !dbg !848
  %18 = extractvalue { ptr, i32 } %17, 0, !dbg !848
  store ptr %18, ptr %5, align 8, !dbg !848
  %19 = extractvalue { ptr, i32 } %17, 1, !dbg !848
  store i32 %19, ptr %6, align 4, !dbg !848
  call void @_ZN4testD2Ev(ptr noundef nonnull align 1 dereferenceable(1) %4) #7, !dbg !847
  br label %27, !dbg !847

20:                                               ; preds = %7
  %21 = load i32, ptr %2, align 4, !dbg !849
  %22 = icmp eq i32 %21, 2, !dbg !851
  br i1 %22, label %23, label %25, !dbg !852

23:                                               ; preds = %20
  %24 = call ptr @__cxa_allocate_exception(i64 8) #7, !dbg !853
  store double 3.140000e+00, ptr %24, align 16, !dbg !853
  invoke void @__cxa_throw(ptr %24, ptr @_ZTId, ptr null) #8
          to label %33 unwind label %16, !dbg !853

25:                                               ; preds = %20
  %26 = load i32, ptr %2, align 4, !dbg !855
  call void @_ZN4testD2Ev(ptr noundef nonnull align 1 dereferenceable(1) %4) #7, !dbg !847
  call void @_ZN4testD2Ev(ptr noundef nonnull align 1 dereferenceable(1) %3) #7, !dbg !847
  ret i32 %26, !dbg !847

27:                                               ; preds = %16, %12
  call void @_ZN4testD2Ev(ptr noundef nonnull align 1 dereferenceable(1) %3) #7, !dbg !847
  br label %28, !dbg !847

28:                                               ; preds = %27
  %29 = load ptr, ptr %5, align 8, !dbg !847
  %30 = load i32, ptr %6, align 4, !dbg !847
  %31 = insertvalue { ptr, i32 } poison, ptr %29, 0, !dbg !847
  %32 = insertvalue { ptr, i32 } %31, i32 %30, 1, !dbg !847
  resume { ptr, i32 } %32, !dbg !847

33:                                               ; preds = %23, %10
  unreachable
}

declare i32 @__gxx_personality_v0(...)

; Function Attrs: nounwind memory(none)
declare i32 @llvm.eh.typeid.for(ptr) #4

declare ptr @__cxa_begin_catch(ptr)

declare void @__cxa_end_catch()

; Function Attrs: mustprogress noinline optnone uwtable
define linkonce_odr dso_local void @_ZN4testC2Ev(ptr noundef nonnull align 1 dereferenceable(1) %0) unnamed_addr #3 comdat align 2 !dbg !856 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.dbg.declare(metadata ptr %2, metadata !857, metadata !DIExpression()), !dbg !859
  %3 = load ptr, ptr %2, align 8
  %4 = call noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(ptr noundef nonnull align 8 dereferenceable(8) @_ZSt4cout, ptr noundef @.str), !dbg !860
  ret void, !dbg !862
}

declare ptr @__cxa_allocate_exception(i64)

declare void @__cxa_throw(ptr, ptr, ptr)

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN4testD2Ev(ptr noundef nonnull align 1 dereferenceable(1) %0) unnamed_addr #5 comdat align 2 personality ptr @__gxx_personality_v0 !dbg !863 {
  %2 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  call void @llvm.dbg.declare(metadata ptr %2, metadata !864, metadata !DIExpression()), !dbg !865
  %3 = load ptr, ptr %2, align 8
  %4 = invoke noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(ptr noundef nonnull align 8 dereferenceable(8) @_ZSt4cout, ptr noundef @.str.1)
          to label %5 unwind label %6, !dbg !866

5:                                                ; preds = %1
  ret void, !dbg !868

6:                                                ; preds = %1
  %7 = landingpad { ptr, i32 }
          catch ptr null, !dbg !866
  %8 = extractvalue { ptr, i32 } %7, 0, !dbg !866
  call void @__clang_call_terminate(ptr %8) #9, !dbg !866
  unreachable, !dbg !866
}

declare noundef nonnull align 8 dereferenceable(8) ptr @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(ptr noundef nonnull align 8 dereferenceable(8), ptr noundef) #2

; Function Attrs: noinline noreturn nounwind uwtable
define linkonce_odr hidden void @__clang_call_terminate(ptr noundef %0) #6 comdat {
  %2 = call ptr @__cxa_begin_catch(ptr %0) #7
  call void @_ZSt9terminatev() #9
  unreachable
}

declare void @_ZSt9terminatev()

attributes #0 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { mustprogress noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind memory(none) }
attributes #5 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { noinline noreturn nounwind uwtable "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { nounwind }
attributes #8 = { noreturn }
attributes #9 = { noreturn nounwind }

!llvm.dbg.cu = !{!13}
!llvm.module.flags = !{!795, !796, !797, !798, !799, !800, !801}
!llvm.ident = !{!802}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(scope: null, file: !2, line: 9, type: !3, isLocal: true, isDefinition: true)
!2 = !DIFile(filename: "./ex01.cpp", directory: "/home/nikita/sibsutis/semester_7/KT/compilers-fall-lec5", checksumkind: CSK_MD5, checksum: "92fd679100e8cbb30859ef06752284a0")
!3 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 144, elements: !6)
!4 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !5)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{!7}
!7 = !DISubrange(count: 18)
!8 = !DIGlobalVariableExpression(var: !9, expr: !DIExpression())
!9 = distinct !DIGlobalVariable(scope: null, file: !2, line: 13, type: !10, isLocal: true, isDefinition: true)
!10 = !DICompositeType(tag: DW_TAG_array_type, baseType: !4, size: 136, elements: !11)
!11 = !{!12}
!12 = !DISubrange(count: 17)
!13 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !14, producer: "Ubuntu clang version 18.1.3 (1ubuntu1)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, retainedTypes: !15, globals: !23, imports: !24, splitDebugInlining: false, nameTableKind: None)
!14 = !DIFile(filename: "ex01.cpp", directory: "/home/nikita/sibsutis/semester_7/KT/compilers-fall-lec5", checksumkind: CSK_MD5, checksum: "92fd679100e8cbb30859ef06752284a0")
!15 = !{!16}
!16 = distinct !DICompositeType(tag: DW_TAG_class_type, name: "test", file: !2, line: 5, size: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !17, identifier: "_ZTS4test")
!17 = !{!18, !22}
!18 = !DISubprogram(name: "test", scope: !16, file: !2, line: 8, type: !19, scopeLine: 8, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!19 = !DISubroutineType(types: !20)
!20 = !{null, !21}
!21 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !16, size: 64, flags: DIFlagArtificial | DIFlagObjectPointer)
!22 = !DISubprogram(name: "~test", scope: !16, file: !2, line: 11, type: !19, scopeLine: 11, flags: DIFlagPublic | DIFlagPrototyped, spFlags: 0)
!23 = !{!0, !8}
!24 = !{!25, !45, !48, !53, !61, !69, !73, !80, !84, !88, !90, !92, !96, !107, !111, !117, !123, !125, !129, !133, !137, !141, !153, !155, !159, !163, !167, !169, !175, !179, !183, !185, !187, !191, !199, !203, !207, !211, !213, !219, !221, !228, !233, !237, !242, !246, !250, !254, !256, !258, !262, !266, !270, !272, !276, !280, !282, !284, !288, !294, !299, !304, !305, !306, !307, !308, !309, !310, !311, !312, !313, !314, !318, !322, !327, !331, !335, !340, !346, !348, !350, !352, !354, !356, !358, !360, !362, !364, !366, !368, !370, !372, !376, !380, !384, !390, !394, !398, !403, !405, !409, !413, !417, !427, !429, !433, !437, !441, !445, !449, !453, !457, !461, !465, !469, !473, !475, !479, !483, !487, !493, !497, !501, !503, !507, !511, !517, !519, !523, !527, !531, !535, !539, !543, !547, !548, !549, !550, !552, !553, !554, !555, !556, !557, !558, !562, !568, !573, !577, !579, !581, !583, !585, !592, !596, !600, !604, !608, !612, !617, !621, !623, !627, !633, !637, !642, !644, !646, !650, !654, !656, !658, !660, !662, !666, !668, !670, !674, !678, !682, !686, !690, !694, !696, !700, !704, !708, !712, !714, !716, !720, !724, !725, !726, !727, !728, !729, !737, !745, !748, !749, !751, !753, !755, !757, !761, !763, !765, !767, !769, !771, !773, !775, !777, !781, !785, !787, !791}
!25 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !27, file: !44, line: 64)
!26 = !DINamespace(name: "std", scope: null)
!27 = !DIDerivedType(tag: DW_TAG_typedef, name: "mbstate_t", file: !28, line: 6, baseType: !29)
!28 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/types/mbstate_t.h", directory: "", checksumkind: CSK_MD5, checksum: "ba8742313715e20e434cf6ccb2db98e3")
!29 = !DIDerivedType(tag: DW_TAG_typedef, name: "__mbstate_t", file: !30, line: 21, baseType: !31)
!30 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h", directory: "", checksumkind: CSK_MD5, checksum: "82911a3e689448e3691ded3e0b471a55")
!31 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !30, line: 13, size: 64, flags: DIFlagTypePassByValue, elements: !32, identifier: "_ZTS11__mbstate_t")
!32 = !{!33, !35}
!33 = !DIDerivedType(tag: DW_TAG_member, name: "__count", scope: !31, file: !30, line: 15, baseType: !34, size: 32)
!34 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!35 = !DIDerivedType(tag: DW_TAG_member, name: "__value", scope: !31, file: !30, line: 20, baseType: !36, size: 32, offset: 32)
!36 = distinct !DICompositeType(tag: DW_TAG_union_type, scope: !31, file: !30, line: 16, size: 32, flags: DIFlagTypePassByValue, elements: !37, identifier: "_ZTSN11__mbstate_tUt_E")
!37 = !{!38, !40}
!38 = !DIDerivedType(tag: DW_TAG_member, name: "__wch", scope: !36, file: !30, line: 18, baseType: !39, size: 32)
!39 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!40 = !DIDerivedType(tag: DW_TAG_member, name: "__wchb", scope: !36, file: !30, line: 19, baseType: !41, size: 32)
!41 = !DICompositeType(tag: DW_TAG_array_type, baseType: !5, size: 32, elements: !42)
!42 = !{!43}
!43 = !DISubrange(count: 4)
!44 = !DIFile(filename: "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/cwchar", directory: "")
!45 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !46, file: !44, line: 141)
!46 = !DIDerivedType(tag: DW_TAG_typedef, name: "wint_t", file: !47, line: 20, baseType: !39)
!47 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/types/wint_t.h", directory: "", checksumkind: CSK_MD5, checksum: "aa31b53ef28dc23152ceb41e2763ded3")
!48 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !49, file: !44, line: 143)
!49 = !DISubprogram(name: "btowc", scope: !50, file: !50, line: 309, type: !51, flags: DIFlagPrototyped, spFlags: 0)
!50 = !DIFile(filename: "/usr/include/wchar.h", directory: "", checksumkind: CSK_MD5, checksum: "889114206ea781a9a9a0b33e52589e47")
!51 = !DISubroutineType(types: !52)
!52 = !{!46, !34}
!53 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !54, file: !44, line: 144)
!54 = !DISubprogram(name: "fgetwc", scope: !50, file: !50, line: 935, type: !55, flags: DIFlagPrototyped, spFlags: 0)
!55 = !DISubroutineType(types: !56)
!56 = !{!46, !57}
!57 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !58, size: 64)
!58 = !DIDerivedType(tag: DW_TAG_typedef, name: "__FILE", file: !59, line: 5, baseType: !60)
!59 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/types/__FILE.h", directory: "", checksumkind: CSK_MD5, checksum: "72a8fe90981f484acae7c6f3dfc5c2b7")
!60 = !DICompositeType(tag: DW_TAG_structure_type, name: "_IO_FILE", file: !59, line: 4, flags: DIFlagFwdDecl | DIFlagNonTrivial, identifier: "_ZTS8_IO_FILE")
!61 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !62, file: !44, line: 145)
!62 = !DISubprogram(name: "fgetws", scope: !50, file: !50, line: 964, type: !63, flags: DIFlagPrototyped, spFlags: 0)
!63 = !DISubroutineType(types: !64)
!64 = !{!65, !67, !34, !68}
!65 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !66, size: 64)
!66 = !DIBasicType(name: "wchar_t", size: 32, encoding: DW_ATE_signed)
!67 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !65)
!68 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !57)
!69 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !70, file: !44, line: 146)
!70 = !DISubprogram(name: "fputwc", scope: !50, file: !50, line: 949, type: !71, flags: DIFlagPrototyped, spFlags: 0)
!71 = !DISubroutineType(types: !72)
!72 = !{!46, !66, !57}
!73 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !74, file: !44, line: 147)
!74 = !DISubprogram(name: "fputws", scope: !50, file: !50, line: 971, type: !75, flags: DIFlagPrototyped, spFlags: 0)
!75 = !DISubroutineType(types: !76)
!76 = !{!34, !77, !68}
!77 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !78)
!78 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !79, size: 64)
!79 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !66)
!80 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !81, file: !44, line: 148)
!81 = !DISubprogram(name: "fwide", scope: !50, file: !50, line: 725, type: !82, flags: DIFlagPrototyped, spFlags: 0)
!82 = !DISubroutineType(types: !83)
!83 = !{!34, !57, !34}
!84 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !85, file: !44, line: 149)
!85 = !DISubprogram(name: "fwprintf", scope: !50, file: !50, line: 732, type: !86, flags: DIFlagPrototyped, spFlags: 0)
!86 = !DISubroutineType(types: !87)
!87 = !{!34, !68, !77, null}
!88 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !89, file: !44, line: 150)
!89 = !DISubprogram(name: "fwscanf", linkageName: "__isoc23_fwscanf", scope: !50, file: !50, line: 795, type: !86, flags: DIFlagPrototyped, spFlags: 0)
!90 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !91, file: !44, line: 151)
!91 = !DISubprogram(name: "getwc", scope: !50, file: !50, line: 936, type: !55, flags: DIFlagPrototyped, spFlags: 0)
!92 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !93, file: !44, line: 152)
!93 = !DISubprogram(name: "getwchar", scope: !50, file: !50, line: 942, type: !94, flags: DIFlagPrototyped, spFlags: 0)
!94 = !DISubroutineType(types: !95)
!95 = !{!46}
!96 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !97, file: !44, line: 153)
!97 = !DISubprogram(name: "mbrlen", scope: !50, file: !50, line: 332, type: !98, flags: DIFlagPrototyped, spFlags: 0)
!98 = !DISubroutineType(types: !99)
!99 = !{!100, !103, !100, !105}
!100 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !101, line: 18, baseType: !102)
!101 = !DIFile(filename: "/usr/lib/llvm-18/lib/clang/18/include/__stddef_size_t.h", directory: "", checksumkind: CSK_MD5, checksum: "2c44e821a2b1951cde2eb0fb2e656867")
!102 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!103 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !104)
!104 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64)
!105 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !106)
!106 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !27, size: 64)
!107 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !108, file: !44, line: 154)
!108 = !DISubprogram(name: "mbrtowc", scope: !50, file: !50, line: 321, type: !109, flags: DIFlagPrototyped, spFlags: 0)
!109 = !DISubroutineType(types: !110)
!110 = !{!100, !67, !103, !100, !105}
!111 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !112, file: !44, line: 155)
!112 = !DISubprogram(name: "mbsinit", scope: !50, file: !50, line: 317, type: !113, flags: DIFlagPrototyped, spFlags: 0)
!113 = !DISubroutineType(types: !114)
!114 = !{!34, !115}
!115 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !116, size: 64)
!116 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !27)
!117 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !118, file: !44, line: 156)
!118 = !DISubprogram(name: "mbsrtowcs", scope: !50, file: !50, line: 362, type: !119, flags: DIFlagPrototyped, spFlags: 0)
!119 = !DISubroutineType(types: !120)
!120 = !{!100, !67, !121, !100, !105}
!121 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !122)
!122 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !104, size: 64)
!123 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !124, file: !44, line: 157)
!124 = !DISubprogram(name: "putwc", scope: !50, file: !50, line: 950, type: !71, flags: DIFlagPrototyped, spFlags: 0)
!125 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !126, file: !44, line: 158)
!126 = !DISubprogram(name: "putwchar", scope: !50, file: !50, line: 956, type: !127, flags: DIFlagPrototyped, spFlags: 0)
!127 = !DISubroutineType(types: !128)
!128 = !{!46, !66}
!129 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !130, file: !44, line: 160)
!130 = !DISubprogram(name: "swprintf", scope: !50, file: !50, line: 742, type: !131, flags: DIFlagPrototyped, spFlags: 0)
!131 = !DISubroutineType(types: !132)
!132 = !{!34, !67, !100, !77, null}
!133 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !134, file: !44, line: 162)
!134 = !DISubprogram(name: "swscanf", linkageName: "__isoc23_swscanf", scope: !50, file: !50, line: 802, type: !135, flags: DIFlagPrototyped, spFlags: 0)
!135 = !DISubroutineType(types: !136)
!136 = !{!34, !77, !77, null}
!137 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !138, file: !44, line: 163)
!138 = !DISubprogram(name: "ungetwc", scope: !50, file: !50, line: 979, type: !139, flags: DIFlagPrototyped, spFlags: 0)
!139 = !DISubroutineType(types: !140)
!140 = !{!46, !46, !57}
!141 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !142, file: !44, line: 164)
!142 = !DISubprogram(name: "vfwprintf", scope: !50, file: !50, line: 750, type: !143, flags: DIFlagPrototyped, spFlags: 0)
!143 = !DISubroutineType(types: !144)
!144 = !{!34, !68, !77, !145}
!145 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !146, size: 64)
!146 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__va_list_tag", size: 192, flags: DIFlagTypePassByValue, elements: !147, identifier: "_ZTS13__va_list_tag")
!147 = !{!148, !149, !150, !152}
!148 = !DIDerivedType(tag: DW_TAG_member, name: "gp_offset", scope: !146, file: !14, baseType: !39, size: 32)
!149 = !DIDerivedType(tag: DW_TAG_member, name: "fp_offset", scope: !146, file: !14, baseType: !39, size: 32, offset: 32)
!150 = !DIDerivedType(tag: DW_TAG_member, name: "overflow_arg_area", scope: !146, file: !14, baseType: !151, size: 64, offset: 64)
!151 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!152 = !DIDerivedType(tag: DW_TAG_member, name: "reg_save_area", scope: !146, file: !14, baseType: !151, size: 64, offset: 128)
!153 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !154, file: !44, line: 166)
!154 = !DISubprogram(name: "vfwscanf", linkageName: "__isoc23_vfwscanf", scope: !50, file: !50, line: 875, type: !143, flags: DIFlagPrototyped, spFlags: 0)
!155 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !156, file: !44, line: 169)
!156 = !DISubprogram(name: "vswprintf", scope: !50, file: !50, line: 763, type: !157, flags: DIFlagPrototyped, spFlags: 0)
!157 = !DISubroutineType(types: !158)
!158 = !{!34, !67, !100, !77, !145}
!159 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !160, file: !44, line: 172)
!160 = !DISubprogram(name: "vswscanf", linkageName: "__isoc23_vswscanf", scope: !50, file: !50, line: 882, type: !161, flags: DIFlagPrototyped, spFlags: 0)
!161 = !DISubroutineType(types: !162)
!162 = !{!34, !77, !77, !145}
!163 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !164, file: !44, line: 174)
!164 = !DISubprogram(name: "vwprintf", scope: !50, file: !50, line: 758, type: !165, flags: DIFlagPrototyped, spFlags: 0)
!165 = !DISubroutineType(types: !166)
!166 = !{!34, !77, !145}
!167 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !168, file: !44, line: 176)
!168 = !DISubprogram(name: "vwscanf", linkageName: "__isoc23_vwscanf", scope: !50, file: !50, line: 879, type: !165, flags: DIFlagPrototyped, spFlags: 0)
!169 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !170, file: !44, line: 178)
!170 = !DISubprogram(name: "wcrtomb", scope: !50, file: !50, line: 326, type: !171, flags: DIFlagPrototyped, spFlags: 0)
!171 = !DISubroutineType(types: !172)
!172 = !{!100, !173, !66, !105}
!173 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !174)
!174 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!175 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !176, file: !44, line: 179)
!176 = !DISubprogram(name: "wcscat", scope: !50, file: !50, line: 121, type: !177, flags: DIFlagPrototyped, spFlags: 0)
!177 = !DISubroutineType(types: !178)
!178 = !{!65, !67, !77}
!179 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !180, file: !44, line: 180)
!180 = !DISubprogram(name: "wcscmp", scope: !50, file: !50, line: 130, type: !181, flags: DIFlagPrototyped, spFlags: 0)
!181 = !DISubroutineType(types: !182)
!182 = !{!34, !78, !78}
!183 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !184, file: !44, line: 181)
!184 = !DISubprogram(name: "wcscoll", scope: !50, file: !50, line: 155, type: !181, flags: DIFlagPrototyped, spFlags: 0)
!185 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !186, file: !44, line: 182)
!186 = !DISubprogram(name: "wcscpy", scope: !50, file: !50, line: 98, type: !177, flags: DIFlagPrototyped, spFlags: 0)
!187 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !188, file: !44, line: 183)
!188 = !DISubprogram(name: "wcscspn", scope: !50, file: !50, line: 212, type: !189, flags: DIFlagPrototyped, spFlags: 0)
!189 = !DISubroutineType(types: !190)
!190 = !{!100, !78, !78}
!191 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !192, file: !44, line: 184)
!192 = !DISubprogram(name: "wcsftime", scope: !50, file: !50, line: 1043, type: !193, flags: DIFlagPrototyped, spFlags: 0)
!193 = !DISubroutineType(types: !194)
!194 = !{!100, !67, !100, !77, !195}
!195 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !196)
!196 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !197, size: 64)
!197 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !198)
!198 = !DICompositeType(tag: DW_TAG_structure_type, name: "tm", file: !50, line: 94, flags: DIFlagFwdDecl | DIFlagNonTrivial, identifier: "_ZTS2tm")
!199 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !200, file: !44, line: 185)
!200 = !DISubprogram(name: "wcslen", scope: !50, file: !50, line: 247, type: !201, flags: DIFlagPrototyped, spFlags: 0)
!201 = !DISubroutineType(types: !202)
!202 = !{!100, !78}
!203 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !204, file: !44, line: 186)
!204 = !DISubprogram(name: "wcsncat", scope: !50, file: !50, line: 125, type: !205, flags: DIFlagPrototyped, spFlags: 0)
!205 = !DISubroutineType(types: !206)
!206 = !{!65, !67, !77, !100}
!207 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !208, file: !44, line: 187)
!208 = !DISubprogram(name: "wcsncmp", scope: !50, file: !50, line: 133, type: !209, flags: DIFlagPrototyped, spFlags: 0)
!209 = !DISubroutineType(types: !210)
!210 = !{!34, !78, !78, !100}
!211 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !212, file: !44, line: 188)
!212 = !DISubprogram(name: "wcsncpy", scope: !50, file: !50, line: 103, type: !205, flags: DIFlagPrototyped, spFlags: 0)
!213 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !214, file: !44, line: 189)
!214 = !DISubprogram(name: "wcsrtombs", scope: !50, file: !50, line: 368, type: !215, flags: DIFlagPrototyped, spFlags: 0)
!215 = !DISubroutineType(types: !216)
!216 = !{!100, !173, !217, !100, !105}
!217 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !218)
!218 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !78, size: 64)
!219 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !220, file: !44, line: 190)
!220 = !DISubprogram(name: "wcsspn", scope: !50, file: !50, line: 216, type: !189, flags: DIFlagPrototyped, spFlags: 0)
!221 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !222, file: !44, line: 191)
!222 = !DISubprogram(name: "wcstod", scope: !50, file: !50, line: 402, type: !223, flags: DIFlagPrototyped, spFlags: 0)
!223 = !DISubroutineType(types: !224)
!224 = !{!225, !77, !226}
!225 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!226 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !227)
!227 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !65, size: 64)
!228 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !229, file: !44, line: 193)
!229 = !DISubprogram(name: "wcstof", scope: !50, file: !50, line: 407, type: !230, flags: DIFlagPrototyped, spFlags: 0)
!230 = !DISubroutineType(types: !231)
!231 = !{!232, !77, !226}
!232 = !DIBasicType(name: "float", size: 32, encoding: DW_ATE_float)
!233 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !234, file: !44, line: 195)
!234 = !DISubprogram(name: "wcstok", scope: !50, file: !50, line: 242, type: !235, flags: DIFlagPrototyped, spFlags: 0)
!235 = !DISubroutineType(types: !236)
!236 = !{!65, !67, !77, !226}
!237 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !238, file: !44, line: 196)
!238 = !DISubprogram(name: "wcstol", linkageName: "__isoc23_wcstol", scope: !50, file: !50, line: 500, type: !239, flags: DIFlagPrototyped, spFlags: 0)
!239 = !DISubroutineType(types: !240)
!240 = !{!241, !77, !226, !34}
!241 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!242 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !243, file: !44, line: 197)
!243 = !DISubprogram(name: "wcstoul", linkageName: "__isoc23_wcstoul", scope: !50, file: !50, line: 503, type: !244, flags: DIFlagPrototyped, spFlags: 0)
!244 = !DISubroutineType(types: !245)
!245 = !{!102, !77, !226, !34}
!246 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !247, file: !44, line: 198)
!247 = !DISubprogram(name: "wcsxfrm", scope: !50, file: !50, line: 159, type: !248, flags: DIFlagPrototyped, spFlags: 0)
!248 = !DISubroutineType(types: !249)
!249 = !{!100, !67, !77, !100}
!250 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !251, file: !44, line: 199)
!251 = !DISubprogram(name: "wctob", scope: !50, file: !50, line: 313, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!252 = !DISubroutineType(types: !253)
!253 = !{!34, !46}
!254 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !255, file: !44, line: 200)
!255 = !DISubprogram(name: "wmemcmp", scope: !50, file: !50, line: 283, type: !209, flags: DIFlagPrototyped, spFlags: 0)
!256 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !257, file: !44, line: 201)
!257 = !DISubprogram(name: "wmemcpy", scope: !50, file: !50, line: 287, type: !205, flags: DIFlagPrototyped, spFlags: 0)
!258 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !259, file: !44, line: 202)
!259 = !DISubprogram(name: "wmemmove", scope: !50, file: !50, line: 292, type: !260, flags: DIFlagPrototyped, spFlags: 0)
!260 = !DISubroutineType(types: !261)
!261 = !{!65, !65, !78, !100}
!262 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !263, file: !44, line: 203)
!263 = !DISubprogram(name: "wmemset", scope: !50, file: !50, line: 296, type: !264, flags: DIFlagPrototyped, spFlags: 0)
!264 = !DISubroutineType(types: !265)
!265 = !{!65, !65, !66, !100}
!266 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !267, file: !44, line: 204)
!267 = !DISubprogram(name: "wprintf", scope: !50, file: !50, line: 739, type: !268, flags: DIFlagPrototyped, spFlags: 0)
!268 = !DISubroutineType(types: !269)
!269 = !{!34, !77, null}
!270 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !271, file: !44, line: 205)
!271 = !DISubprogram(name: "wscanf", linkageName: "__isoc23_wscanf", scope: !50, file: !50, line: 799, type: !268, flags: DIFlagPrototyped, spFlags: 0)
!272 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !273, file: !44, line: 206)
!273 = !DISubprogram(name: "wcschr", scope: !50, file: !50, line: 189, type: !274, flags: DIFlagPrototyped, spFlags: 0)
!274 = !DISubroutineType(types: !275)
!275 = !{!65, !78, !66}
!276 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !277, file: !44, line: 207)
!277 = !DISubprogram(name: "wcspbrk", scope: !50, file: !50, line: 226, type: !278, flags: DIFlagPrototyped, spFlags: 0)
!278 = !DISubroutineType(types: !279)
!279 = !{!65, !78, !78}
!280 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !281, file: !44, line: 208)
!281 = !DISubprogram(name: "wcsrchr", scope: !50, file: !50, line: 199, type: !274, flags: DIFlagPrototyped, spFlags: 0)
!282 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !283, file: !44, line: 209)
!283 = !DISubprogram(name: "wcsstr", scope: !50, file: !50, line: 237, type: !278, flags: DIFlagPrototyped, spFlags: 0)
!284 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !285, file: !44, line: 210)
!285 = !DISubprogram(name: "wmemchr", scope: !50, file: !50, line: 278, type: !286, flags: DIFlagPrototyped, spFlags: 0)
!286 = !DISubroutineType(types: !287)
!287 = !{!65, !78, !66, !100}
!288 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !290, file: !44, line: 251)
!289 = !DINamespace(name: "__gnu_cxx", scope: null)
!290 = !DISubprogram(name: "wcstold", scope: !50, file: !50, line: 409, type: !291, flags: DIFlagPrototyped, spFlags: 0)
!291 = !DISubroutineType(types: !292)
!292 = !{!293, !77, !226}
!293 = !DIBasicType(name: "long double", size: 128, encoding: DW_ATE_float)
!294 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !295, file: !44, line: 260)
!295 = !DISubprogram(name: "wcstoll", linkageName: "__isoc23_wcstoll", scope: !50, file: !50, line: 508, type: !296, flags: DIFlagPrototyped, spFlags: 0)
!296 = !DISubroutineType(types: !297)
!297 = !{!298, !77, !226, !34}
!298 = !DIBasicType(name: "long long", size: 64, encoding: DW_ATE_signed)
!299 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !300, file: !44, line: 261)
!300 = !DISubprogram(name: "wcstoull", linkageName: "__isoc23_wcstoull", scope: !50, file: !50, line: 513, type: !301, flags: DIFlagPrototyped, spFlags: 0)
!301 = !DISubroutineType(types: !302)
!302 = !{!303, !77, !226, !34}
!303 = !DIBasicType(name: "unsigned long long", size: 64, encoding: DW_ATE_unsigned)
!304 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !290, file: !44, line: 267)
!305 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !295, file: !44, line: 268)
!306 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !300, file: !44, line: 269)
!307 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !229, file: !44, line: 283)
!308 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !154, file: !44, line: 286)
!309 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !160, file: !44, line: 289)
!310 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !168, file: !44, line: 292)
!311 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !290, file: !44, line: 296)
!312 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !295, file: !44, line: 297)
!313 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !300, file: !44, line: 298)
!314 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !315, file: !316, line: 66)
!315 = !DICompositeType(tag: DW_TAG_class_type, name: "exception_ptr", scope: !317, file: !316, line: 97, size: 64, flags: DIFlagFwdDecl | DIFlagNonTrivial, identifier: "_ZTSNSt15__exception_ptr13exception_ptrE")
!316 = !DIFile(filename: "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/exception_ptr.h", directory: "", checksumkind: CSK_MD5, checksum: "314ad14748ccb9ff85c65d17ebb0828b")
!317 = !DINamespace(name: "__exception_ptr", scope: !26)
!318 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !317, entity: !319, file: !316, line: 85)
!319 = !DISubprogram(name: "rethrow_exception", linkageName: "_ZSt17rethrow_exceptionNSt15__exception_ptr13exception_ptrE", scope: !26, file: !316, line: 81, type: !320, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!320 = !DISubroutineType(types: !321)
!321 = !{null, !315}
!322 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !323, file: !316, line: 243)
!323 = !DISubprogram(name: "swap", linkageName: "_ZNSt15__exception_ptr4swapERNS_13exception_ptrES1_", scope: !317, file: !316, line: 230, type: !324, flags: DIFlagPrototyped, spFlags: 0)
!324 = !DISubroutineType(types: !325)
!325 = !{null, !326, !326}
!326 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !315, size: 64)
!327 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !328, file: !330, line: 53)
!328 = !DICompositeType(tag: DW_TAG_structure_type, name: "lconv", file: !329, line: 51, size: 768, flags: DIFlagFwdDecl, identifier: "_ZTS5lconv")
!329 = !DIFile(filename: "/usr/include/locale.h", directory: "", checksumkind: CSK_MD5, checksum: "23ebf40dea0ab9a74daf64a0eaa99518")
!330 = !DIFile(filename: "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/clocale", directory: "")
!331 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !332, file: !330, line: 54)
!332 = !DISubprogram(name: "setlocale", scope: !329, file: !329, line: 122, type: !333, flags: DIFlagPrototyped, spFlags: 0)
!333 = !DISubroutineType(types: !334)
!334 = !{!174, !34, !104}
!335 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !336, file: !330, line: 55)
!336 = !DISubprogram(name: "localeconv", scope: !329, file: !329, line: 125, type: !337, flags: DIFlagPrototyped, spFlags: 0)
!337 = !DISubroutineType(types: !338)
!338 = !{!339}
!339 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !328, size: 64)
!340 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !341, file: !345, line: 64)
!341 = !DISubprogram(name: "isalnum", scope: !342, file: !342, line: 108, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!342 = !DIFile(filename: "/usr/include/ctype.h", directory: "", checksumkind: CSK_MD5, checksum: "43fd45dcf96e8fb7d8f14700096497c7")
!343 = !DISubroutineType(types: !344)
!344 = !{!34, !34}
!345 = !DIFile(filename: "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/cctype", directory: "")
!346 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !347, file: !345, line: 65)
!347 = !DISubprogram(name: "isalpha", scope: !342, file: !342, line: 109, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!348 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !349, file: !345, line: 66)
!349 = !DISubprogram(name: "iscntrl", scope: !342, file: !342, line: 110, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!350 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !351, file: !345, line: 67)
!351 = !DISubprogram(name: "isdigit", scope: !342, file: !342, line: 111, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!352 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !353, file: !345, line: 68)
!353 = !DISubprogram(name: "isgraph", scope: !342, file: !342, line: 113, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!354 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !355, file: !345, line: 69)
!355 = !DISubprogram(name: "islower", scope: !342, file: !342, line: 112, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!356 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !357, file: !345, line: 70)
!357 = !DISubprogram(name: "isprint", scope: !342, file: !342, line: 114, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!358 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !359, file: !345, line: 71)
!359 = !DISubprogram(name: "ispunct", scope: !342, file: !342, line: 115, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!360 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !361, file: !345, line: 72)
!361 = !DISubprogram(name: "isspace", scope: !342, file: !342, line: 116, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!362 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !363, file: !345, line: 73)
!363 = !DISubprogram(name: "isupper", scope: !342, file: !342, line: 117, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!364 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !365, file: !345, line: 74)
!365 = !DISubprogram(name: "isxdigit", scope: !342, file: !342, line: 118, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!366 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !367, file: !345, line: 75)
!367 = !DISubprogram(name: "tolower", scope: !342, file: !342, line: 122, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!368 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !369, file: !345, line: 76)
!369 = !DISubprogram(name: "toupper", scope: !342, file: !342, line: 125, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!370 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !371, file: !345, line: 87)
!371 = !DISubprogram(name: "isblank", scope: !342, file: !342, line: 130, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!372 = !DIImportedEntity(tag: DW_TAG_imported_module, scope: !373, entity: !374, file: !375, line: 58)
!373 = !DINamespace(name: "__gnu_debug", scope: null)
!374 = !DINamespace(name: "__debug", scope: !26)
!375 = !DIFile(filename: "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/debug/debug.h", directory: "", checksumkind: CSK_MD5, checksum: "752210a319f5f5d356cc29cd1ce3cdc7")
!376 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !377, file: !379, line: 52)
!377 = !DISubprogram(name: "abs", scope: !378, file: !378, line: 980, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!378 = !DIFile(filename: "/usr/include/stdlib.h", directory: "", checksumkind: CSK_MD5, checksum: "7fa2ecb2348a66f8b44ab9a15abd0b72")
!379 = !DIFile(filename: "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/std_abs.h", directory: "")
!380 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !381, file: !383, line: 131)
!381 = !DIDerivedType(tag: DW_TAG_typedef, name: "div_t", file: !378, line: 63, baseType: !382)
!382 = !DICompositeType(tag: DW_TAG_structure_type, file: !378, line: 59, size: 64, flags: DIFlagFwdDecl, identifier: "_ZTS5div_t")
!383 = !DIFile(filename: "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/cstdlib", directory: "")
!384 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !385, file: !383, line: 132)
!385 = !DIDerivedType(tag: DW_TAG_typedef, name: "ldiv_t", file: !378, line: 71, baseType: !386)
!386 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !378, line: 67, size: 128, flags: DIFlagTypePassByValue, elements: !387, identifier: "_ZTS6ldiv_t")
!387 = !{!388, !389}
!388 = !DIDerivedType(tag: DW_TAG_member, name: "quot", scope: !386, file: !378, line: 69, baseType: !241, size: 64)
!389 = !DIDerivedType(tag: DW_TAG_member, name: "rem", scope: !386, file: !378, line: 70, baseType: !241, size: 64, offset: 64)
!390 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !391, file: !383, line: 134)
!391 = !DISubprogram(name: "abort", scope: !378, file: !378, line: 730, type: !392, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!392 = !DISubroutineType(types: !393)
!393 = !{null}
!394 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !395, file: !383, line: 136)
!395 = !DISubprogram(name: "aligned_alloc", scope: !378, file: !378, line: 724, type: !396, flags: DIFlagPrototyped, spFlags: 0)
!396 = !DISubroutineType(types: !397)
!397 = !{!151, !100, !100}
!398 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !399, file: !383, line: 138)
!399 = !DISubprogram(name: "atexit", scope: !378, file: !378, line: 734, type: !400, flags: DIFlagPrototyped, spFlags: 0)
!400 = !DISubroutineType(types: !401)
!401 = !{!34, !402}
!402 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !392, size: 64)
!403 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !404, file: !383, line: 141)
!404 = !DISubprogram(name: "at_quick_exit", scope: !378, file: !378, line: 739, type: !400, flags: DIFlagPrototyped, spFlags: 0)
!405 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !406, file: !383, line: 144)
!406 = !DISubprogram(name: "atof", scope: !378, file: !378, line: 102, type: !407, flags: DIFlagPrototyped, spFlags: 0)
!407 = !DISubroutineType(types: !408)
!408 = !{!225, !104}
!409 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !410, file: !383, line: 145)
!410 = !DISubprogram(name: "atoi", scope: !378, file: !378, line: 105, type: !411, flags: DIFlagPrototyped, spFlags: 0)
!411 = !DISubroutineType(types: !412)
!412 = !{!34, !104}
!413 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !414, file: !383, line: 146)
!414 = !DISubprogram(name: "atol", scope: !378, file: !378, line: 108, type: !415, flags: DIFlagPrototyped, spFlags: 0)
!415 = !DISubroutineType(types: !416)
!416 = !{!241, !104}
!417 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !418, file: !383, line: 147)
!418 = !DISubprogram(name: "bsearch", scope: !378, file: !378, line: 960, type: !419, flags: DIFlagPrototyped, spFlags: 0)
!419 = !DISubroutineType(types: !420)
!420 = !{!151, !421, !421, !100, !100, !423}
!421 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !422, size: 64)
!422 = !DIDerivedType(tag: DW_TAG_const_type, baseType: null)
!423 = !DIDerivedType(tag: DW_TAG_typedef, name: "__compar_fn_t", file: !378, line: 948, baseType: !424)
!424 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !425, size: 64)
!425 = !DISubroutineType(types: !426)
!426 = !{!34, !421, !421}
!427 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !428, file: !383, line: 148)
!428 = !DISubprogram(name: "calloc", scope: !378, file: !378, line: 675, type: !396, flags: DIFlagPrototyped, spFlags: 0)
!429 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !430, file: !383, line: 149)
!430 = !DISubprogram(name: "div", scope: !378, file: !378, line: 992, type: !431, flags: DIFlagPrototyped, spFlags: 0)
!431 = !DISubroutineType(types: !432)
!432 = !{!381, !34, !34}
!433 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !434, file: !383, line: 150)
!434 = !DISubprogram(name: "exit", scope: !378, file: !378, line: 756, type: !435, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!435 = !DISubroutineType(types: !436)
!436 = !{null, !34}
!437 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !438, file: !383, line: 151)
!438 = !DISubprogram(name: "free", scope: !378, file: !378, line: 687, type: !439, flags: DIFlagPrototyped, spFlags: 0)
!439 = !DISubroutineType(types: !440)
!440 = !{null, !151}
!441 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !442, file: !383, line: 152)
!442 = !DISubprogram(name: "getenv", scope: !378, file: !378, line: 773, type: !443, flags: DIFlagPrototyped, spFlags: 0)
!443 = !DISubroutineType(types: !444)
!444 = !{!174, !104}
!445 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !446, file: !383, line: 153)
!446 = !DISubprogram(name: "labs", scope: !378, file: !378, line: 981, type: !447, flags: DIFlagPrototyped, spFlags: 0)
!447 = !DISubroutineType(types: !448)
!448 = !{!241, !241}
!449 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !450, file: !383, line: 154)
!450 = !DISubprogram(name: "ldiv", scope: !378, file: !378, line: 994, type: !451, flags: DIFlagPrototyped, spFlags: 0)
!451 = !DISubroutineType(types: !452)
!452 = !{!385, !241, !241}
!453 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !454, file: !383, line: 155)
!454 = !DISubprogram(name: "malloc", scope: !378, file: !378, line: 672, type: !455, flags: DIFlagPrototyped, spFlags: 0)
!455 = !DISubroutineType(types: !456)
!456 = !{!151, !100}
!457 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !458, file: !383, line: 157)
!458 = !DISubprogram(name: "mblen", scope: !378, file: !378, line: 1062, type: !459, flags: DIFlagPrototyped, spFlags: 0)
!459 = !DISubroutineType(types: !460)
!460 = !{!34, !104, !100}
!461 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !462, file: !383, line: 158)
!462 = !DISubprogram(name: "mbstowcs", scope: !378, file: !378, line: 1073, type: !463, flags: DIFlagPrototyped, spFlags: 0)
!463 = !DISubroutineType(types: !464)
!464 = !{!100, !67, !103, !100}
!465 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !466, file: !383, line: 159)
!466 = !DISubprogram(name: "mbtowc", scope: !378, file: !378, line: 1065, type: !467, flags: DIFlagPrototyped, spFlags: 0)
!467 = !DISubroutineType(types: !468)
!468 = !{!34, !67, !103, !100}
!469 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !470, file: !383, line: 161)
!470 = !DISubprogram(name: "qsort", scope: !378, file: !378, line: 970, type: !471, flags: DIFlagPrototyped, spFlags: 0)
!471 = !DISubroutineType(types: !472)
!472 = !{null, !151, !100, !100, !423}
!473 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !474, file: !383, line: 164)
!474 = !DISubprogram(name: "quick_exit", scope: !378, file: !378, line: 762, type: !435, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!475 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !476, file: !383, line: 167)
!476 = !DISubprogram(name: "rand", scope: !378, file: !378, line: 573, type: !477, flags: DIFlagPrototyped, spFlags: 0)
!477 = !DISubroutineType(types: !478)
!478 = !{!34}
!479 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !480, file: !383, line: 168)
!480 = !DISubprogram(name: "realloc", scope: !378, file: !378, line: 683, type: !481, flags: DIFlagPrototyped, spFlags: 0)
!481 = !DISubroutineType(types: !482)
!482 = !{!151, !151, !100}
!483 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !484, file: !383, line: 169)
!484 = !DISubprogram(name: "srand", scope: !378, file: !378, line: 575, type: !485, flags: DIFlagPrototyped, spFlags: 0)
!485 = !DISubroutineType(types: !486)
!486 = !{null, !39}
!487 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !488, file: !383, line: 170)
!488 = !DISubprogram(name: "strtod", scope: !378, file: !378, line: 118, type: !489, flags: DIFlagPrototyped, spFlags: 0)
!489 = !DISubroutineType(types: !490)
!490 = !{!225, !103, !491}
!491 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !492)
!492 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !174, size: 64)
!493 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !494, file: !383, line: 171)
!494 = !DISubprogram(name: "strtol", linkageName: "__isoc23_strtol", scope: !378, file: !378, line: 215, type: !495, flags: DIFlagPrototyped, spFlags: 0)
!495 = !DISubroutineType(types: !496)
!496 = !{!241, !103, !491, !34}
!497 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !498, file: !383, line: 172)
!498 = !DISubprogram(name: "strtoul", linkageName: "__isoc23_strtoul", scope: !378, file: !378, line: 219, type: !499, flags: DIFlagPrototyped, spFlags: 0)
!499 = !DISubroutineType(types: !500)
!500 = !{!102, !103, !491, !34}
!501 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !502, file: !383, line: 173)
!502 = !DISubprogram(name: "system", scope: !378, file: !378, line: 923, type: !411, flags: DIFlagPrototyped, spFlags: 0)
!503 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !504, file: !383, line: 175)
!504 = !DISubprogram(name: "wcstombs", scope: !378, file: !378, line: 1077, type: !505, flags: DIFlagPrototyped, spFlags: 0)
!505 = !DISubroutineType(types: !506)
!506 = !{!100, !173, !77, !100}
!507 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !508, file: !383, line: 176)
!508 = !DISubprogram(name: "wctomb", scope: !378, file: !378, line: 1069, type: !509, flags: DIFlagPrototyped, spFlags: 0)
!509 = !DISubroutineType(types: !510)
!510 = !{!34, !174, !66}
!511 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !512, file: !383, line: 204)
!512 = !DIDerivedType(tag: DW_TAG_typedef, name: "lldiv_t", file: !378, line: 81, baseType: !513)
!513 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !378, line: 77, size: 128, flags: DIFlagTypePassByValue, elements: !514, identifier: "_ZTS7lldiv_t")
!514 = !{!515, !516}
!515 = !DIDerivedType(tag: DW_TAG_member, name: "quot", scope: !513, file: !378, line: 79, baseType: !298, size: 64)
!516 = !DIDerivedType(tag: DW_TAG_member, name: "rem", scope: !513, file: !378, line: 80, baseType: !298, size: 64, offset: 64)
!517 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !518, file: !383, line: 210)
!518 = !DISubprogram(name: "_Exit", scope: !378, file: !378, line: 768, type: !435, flags: DIFlagPrototyped | DIFlagNoReturn, spFlags: 0)
!519 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !520, file: !383, line: 214)
!520 = !DISubprogram(name: "llabs", scope: !378, file: !378, line: 984, type: !521, flags: DIFlagPrototyped, spFlags: 0)
!521 = !DISubroutineType(types: !522)
!522 = !{!298, !298}
!523 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !524, file: !383, line: 220)
!524 = !DISubprogram(name: "lldiv", scope: !378, file: !378, line: 998, type: !525, flags: DIFlagPrototyped, spFlags: 0)
!525 = !DISubroutineType(types: !526)
!526 = !{!512, !298, !298}
!527 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !528, file: !383, line: 231)
!528 = !DISubprogram(name: "atoll", scope: !378, file: !378, line: 113, type: !529, flags: DIFlagPrototyped, spFlags: 0)
!529 = !DISubroutineType(types: !530)
!530 = !{!298, !104}
!531 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !532, file: !383, line: 232)
!532 = !DISubprogram(name: "strtoll", linkageName: "__isoc23_strtoll", scope: !378, file: !378, line: 238, type: !533, flags: DIFlagPrototyped, spFlags: 0)
!533 = !DISubroutineType(types: !534)
!534 = !{!298, !103, !491, !34}
!535 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !536, file: !383, line: 233)
!536 = !DISubprogram(name: "strtoull", linkageName: "__isoc23_strtoull", scope: !378, file: !378, line: 243, type: !537, flags: DIFlagPrototyped, spFlags: 0)
!537 = !DISubroutineType(types: !538)
!538 = !{!303, !103, !491, !34}
!539 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !540, file: !383, line: 235)
!540 = !DISubprogram(name: "strtof", scope: !378, file: !378, line: 124, type: !541, flags: DIFlagPrototyped, spFlags: 0)
!541 = !DISubroutineType(types: !542)
!542 = !{!232, !103, !491}
!543 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !544, file: !383, line: 236)
!544 = !DISubprogram(name: "strtold", scope: !378, file: !378, line: 127, type: !545, flags: DIFlagPrototyped, spFlags: 0)
!545 = !DISubroutineType(types: !546)
!546 = !{!293, !103, !491}
!547 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !512, file: !383, line: 244)
!548 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !518, file: !383, line: 246)
!549 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !520, file: !383, line: 248)
!550 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !551, file: !383, line: 249)
!551 = !DISubprogram(name: "div", linkageName: "_ZN9__gnu_cxx3divExx", scope: !289, file: !383, line: 217, type: !525, flags: DIFlagPrototyped, spFlags: 0)
!552 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !524, file: !383, line: 250)
!553 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !528, file: !383, line: 252)
!554 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !540, file: !383, line: 253)
!555 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !532, file: !383, line: 254)
!556 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !536, file: !383, line: 255)
!557 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !544, file: !383, line: 256)
!558 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !559, file: !561, line: 98)
!559 = !DIDerivedType(tag: DW_TAG_typedef, name: "FILE", file: !560, line: 7, baseType: !60)
!560 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/types/FILE.h", directory: "", checksumkind: CSK_MD5, checksum: "571f9fb6223c42439075fdde11a0de5d")
!561 = !DIFile(filename: "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/cstdio", directory: "")
!562 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !563, file: !561, line: 99)
!563 = !DIDerivedType(tag: DW_TAG_typedef, name: "fpos_t", file: !564, line: 85, baseType: !565)
!564 = !DIFile(filename: "/usr/include/stdio.h", directory: "", checksumkind: CSK_MD5, checksum: "1e435c46987a169d9f9186f63a512303")
!565 = !DIDerivedType(tag: DW_TAG_typedef, name: "__fpos_t", file: !566, line: 14, baseType: !567)
!566 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h", directory: "", checksumkind: CSK_MD5, checksum: "32de8bdaf3551a6c0a9394f9af4389ce")
!567 = !DICompositeType(tag: DW_TAG_structure_type, name: "_G_fpos_t", file: !566, line: 10, size: 128, flags: DIFlagFwdDecl, identifier: "_ZTS9_G_fpos_t")
!568 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !569, file: !561, line: 101)
!569 = !DISubprogram(name: "clearerr", scope: !564, file: !564, line: 860, type: !570, flags: DIFlagPrototyped, spFlags: 0)
!570 = !DISubroutineType(types: !571)
!571 = !{null, !572}
!572 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !559, size: 64)
!573 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !574, file: !561, line: 102)
!574 = !DISubprogram(name: "fclose", scope: !564, file: !564, line: 184, type: !575, flags: DIFlagPrototyped, spFlags: 0)
!575 = !DISubroutineType(types: !576)
!576 = !{!34, !572}
!577 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !578, file: !561, line: 103)
!578 = !DISubprogram(name: "feof", scope: !564, file: !564, line: 862, type: !575, flags: DIFlagPrototyped, spFlags: 0)
!579 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !580, file: !561, line: 104)
!580 = !DISubprogram(name: "ferror", scope: !564, file: !564, line: 864, type: !575, flags: DIFlagPrototyped, spFlags: 0)
!581 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !582, file: !561, line: 105)
!582 = !DISubprogram(name: "fflush", scope: !564, file: !564, line: 236, type: !575, flags: DIFlagPrototyped, spFlags: 0)
!583 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !584, file: !561, line: 106)
!584 = !DISubprogram(name: "fgetc", scope: !564, file: !564, line: 575, type: !575, flags: DIFlagPrototyped, spFlags: 0)
!585 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !586, file: !561, line: 107)
!586 = !DISubprogram(name: "fgetpos", scope: !564, file: !564, line: 829, type: !587, flags: DIFlagPrototyped, spFlags: 0)
!587 = !DISubroutineType(types: !588)
!588 = !{!34, !589, !590}
!589 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !572)
!590 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !591)
!591 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !563, size: 64)
!592 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !593, file: !561, line: 108)
!593 = !DISubprogram(name: "fgets", scope: !564, file: !564, line: 654, type: !594, flags: DIFlagPrototyped, spFlags: 0)
!594 = !DISubroutineType(types: !595)
!595 = !{!174, !173, !34, !589}
!596 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !597, file: !561, line: 109)
!597 = !DISubprogram(name: "fopen", scope: !564, file: !564, line: 264, type: !598, flags: DIFlagPrototyped, spFlags: 0)
!598 = !DISubroutineType(types: !599)
!599 = !{!572, !103, !103}
!600 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !601, file: !561, line: 110)
!601 = !DISubprogram(name: "fprintf", scope: !564, file: !564, line: 357, type: !602, flags: DIFlagPrototyped, spFlags: 0)
!602 = !DISubroutineType(types: !603)
!603 = !{!34, !589, !103, null}
!604 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !605, file: !561, line: 111)
!605 = !DISubprogram(name: "fputc", scope: !564, file: !564, line: 611, type: !606, flags: DIFlagPrototyped, spFlags: 0)
!606 = !DISubroutineType(types: !607)
!607 = !{!34, !34, !572}
!608 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !609, file: !561, line: 112)
!609 = !DISubprogram(name: "fputs", scope: !564, file: !564, line: 717, type: !610, flags: DIFlagPrototyped, spFlags: 0)
!610 = !DISubroutineType(types: !611)
!611 = !{!34, !103, !589}
!612 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !613, file: !561, line: 113)
!613 = !DISubprogram(name: "fread", scope: !564, file: !564, line: 738, type: !614, flags: DIFlagPrototyped, spFlags: 0)
!614 = !DISubroutineType(types: !615)
!615 = !{!100, !616, !100, !100, !589}
!616 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !151)
!617 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !618, file: !561, line: 114)
!618 = !DISubprogram(name: "freopen", scope: !564, file: !564, line: 271, type: !619, flags: DIFlagPrototyped, spFlags: 0)
!619 = !DISubroutineType(types: !620)
!620 = !{!572, !103, !103, !589}
!621 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !622, file: !561, line: 115)
!622 = !DISubprogram(name: "fscanf", linkageName: "__isoc23_fscanf", scope: !564, file: !564, line: 442, type: !602, flags: DIFlagPrototyped, spFlags: 0)
!623 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !624, file: !561, line: 116)
!624 = !DISubprogram(name: "fseek", scope: !564, file: !564, line: 779, type: !625, flags: DIFlagPrototyped, spFlags: 0)
!625 = !DISubroutineType(types: !626)
!626 = !{!34, !572, !241, !34}
!627 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !628, file: !561, line: 117)
!628 = !DISubprogram(name: "fsetpos", scope: !564, file: !564, line: 835, type: !629, flags: DIFlagPrototyped, spFlags: 0)
!629 = !DISubroutineType(types: !630)
!630 = !{!34, !572, !631}
!631 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !632, size: 64)
!632 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !563)
!633 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !634, file: !561, line: 118)
!634 = !DISubprogram(name: "ftell", scope: !564, file: !564, line: 785, type: !635, flags: DIFlagPrototyped, spFlags: 0)
!635 = !DISubroutineType(types: !636)
!636 = !{!241, !572}
!637 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !638, file: !561, line: 119)
!638 = !DISubprogram(name: "fwrite", scope: !564, file: !564, line: 745, type: !639, flags: DIFlagPrototyped, spFlags: 0)
!639 = !DISubroutineType(types: !640)
!640 = !{!100, !641, !100, !100, !589}
!641 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !421)
!642 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !643, file: !561, line: 120)
!643 = !DISubprogram(name: "getc", scope: !564, file: !564, line: 576, type: !575, flags: DIFlagPrototyped, spFlags: 0)
!644 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !645, file: !561, line: 121)
!645 = !DISubprogram(name: "getchar", scope: !564, file: !564, line: 582, type: !477, flags: DIFlagPrototyped, spFlags: 0)
!646 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !647, file: !561, line: 126)
!647 = !DISubprogram(name: "perror", scope: !564, file: !564, line: 878, type: !648, flags: DIFlagPrototyped, spFlags: 0)
!648 = !DISubroutineType(types: !649)
!649 = !{null, !104}
!650 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !651, file: !561, line: 127)
!651 = !DISubprogram(name: "printf", scope: !564, file: !564, line: 363, type: !652, flags: DIFlagPrototyped, spFlags: 0)
!652 = !DISubroutineType(types: !653)
!653 = !{!34, !103, null}
!654 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !655, file: !561, line: 128)
!655 = !DISubprogram(name: "putc", scope: !564, file: !564, line: 612, type: !606, flags: DIFlagPrototyped, spFlags: 0)
!656 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !657, file: !561, line: 129)
!657 = !DISubprogram(name: "putchar", scope: !564, file: !564, line: 618, type: !343, flags: DIFlagPrototyped, spFlags: 0)
!658 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !659, file: !561, line: 130)
!659 = !DISubprogram(name: "puts", scope: !564, file: !564, line: 724, type: !411, flags: DIFlagPrototyped, spFlags: 0)
!660 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !661, file: !561, line: 131)
!661 = !DISubprogram(name: "remove", scope: !564, file: !564, line: 158, type: !411, flags: DIFlagPrototyped, spFlags: 0)
!662 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !663, file: !561, line: 132)
!663 = !DISubprogram(name: "rename", scope: !564, file: !564, line: 160, type: !664, flags: DIFlagPrototyped, spFlags: 0)
!664 = !DISubroutineType(types: !665)
!665 = !{!34, !104, !104}
!666 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !667, file: !561, line: 133)
!667 = !DISubprogram(name: "rewind", scope: !564, file: !564, line: 790, type: !570, flags: DIFlagPrototyped, spFlags: 0)
!668 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !669, file: !561, line: 134)
!669 = !DISubprogram(name: "scanf", linkageName: "__isoc23_scanf", scope: !564, file: !564, line: 445, type: !652, flags: DIFlagPrototyped, spFlags: 0)
!670 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !671, file: !561, line: 135)
!671 = !DISubprogram(name: "setbuf", scope: !564, file: !564, line: 334, type: !672, flags: DIFlagPrototyped, spFlags: 0)
!672 = !DISubroutineType(types: !673)
!673 = !{null, !589, !173}
!674 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !675, file: !561, line: 136)
!675 = !DISubprogram(name: "setvbuf", scope: !564, file: !564, line: 339, type: !676, flags: DIFlagPrototyped, spFlags: 0)
!676 = !DISubroutineType(types: !677)
!677 = !{!34, !589, !173, !34, !100}
!678 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !679, file: !561, line: 137)
!679 = !DISubprogram(name: "sprintf", scope: !564, file: !564, line: 365, type: !680, flags: DIFlagPrototyped, spFlags: 0)
!680 = !DISubroutineType(types: !681)
!681 = !{!34, !173, !103, null}
!682 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !683, file: !561, line: 138)
!683 = !DISubprogram(name: "sscanf", linkageName: "__isoc23_sscanf", scope: !564, file: !564, line: 447, type: !684, flags: DIFlagPrototyped, spFlags: 0)
!684 = !DISubroutineType(types: !685)
!685 = !{!34, !103, !103, null}
!686 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !687, file: !561, line: 139)
!687 = !DISubprogram(name: "tmpfile", scope: !564, file: !564, line: 194, type: !688, flags: DIFlagPrototyped, spFlags: 0)
!688 = !DISubroutineType(types: !689)
!689 = !{!572}
!690 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !691, file: !561, line: 141)
!691 = !DISubprogram(name: "tmpnam", scope: !564, file: !564, line: 211, type: !692, flags: DIFlagPrototyped, spFlags: 0)
!692 = !DISubroutineType(types: !693)
!693 = !{!174, !174}
!694 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !695, file: !561, line: 143)
!695 = !DISubprogram(name: "ungetc", scope: !564, file: !564, line: 731, type: !606, flags: DIFlagPrototyped, spFlags: 0)
!696 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !697, file: !561, line: 144)
!697 = !DISubprogram(name: "vfprintf", scope: !564, file: !564, line: 372, type: !698, flags: DIFlagPrototyped, spFlags: 0)
!698 = !DISubroutineType(types: !699)
!699 = !{!34, !589, !103, !145}
!700 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !701, file: !561, line: 145)
!701 = !DISubprogram(name: "vprintf", scope: !564, file: !564, line: 378, type: !702, flags: DIFlagPrototyped, spFlags: 0)
!702 = !DISubroutineType(types: !703)
!703 = !{!34, !103, !145}
!704 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !705, file: !561, line: 146)
!705 = !DISubprogram(name: "vsprintf", scope: !564, file: !564, line: 380, type: !706, flags: DIFlagPrototyped, spFlags: 0)
!706 = !DISubroutineType(types: !707)
!707 = !{!34, !173, !103, !145}
!708 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !709, file: !561, line: 175)
!709 = !DISubprogram(name: "snprintf", scope: !564, file: !564, line: 385, type: !710, flags: DIFlagPrototyped, spFlags: 0)
!710 = !DISubroutineType(types: !711)
!711 = !{!34, !173, !100, !103, null}
!712 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !713, file: !561, line: 176)
!713 = !DISubprogram(name: "vfscanf", linkageName: "__isoc23_vfscanf", scope: !564, file: !564, line: 511, type: !698, flags: DIFlagPrototyped, spFlags: 0)
!714 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !715, file: !561, line: 177)
!715 = !DISubprogram(name: "vscanf", linkageName: "__isoc23_vscanf", scope: !564, file: !564, line: 516, type: !702, flags: DIFlagPrototyped, spFlags: 0)
!716 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !717, file: !561, line: 178)
!717 = !DISubprogram(name: "vsnprintf", scope: !564, file: !564, line: 389, type: !718, flags: DIFlagPrototyped, spFlags: 0)
!718 = !DISubroutineType(types: !719)
!719 = !{!34, !173, !100, !103, !145}
!720 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !289, entity: !721, file: !561, line: 179)
!721 = !DISubprogram(name: "vsscanf", linkageName: "__isoc23_vsscanf", scope: !564, file: !564, line: 519, type: !722, flags: DIFlagPrototyped, spFlags: 0)
!722 = !DISubroutineType(types: !723)
!723 = !{!34, !103, !103, !145}
!724 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !709, file: !561, line: 185)
!725 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !713, file: !561, line: 186)
!726 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !715, file: !561, line: 187)
!727 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !717, file: !561, line: 188)
!728 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !721, file: !561, line: 189)
!729 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !730, file: !736, line: 58)
!730 = !DIDerivedType(tag: DW_TAG_typedef, name: "max_align_t", file: !731, line: 24, baseType: !732)
!731 = !DIFile(filename: "/usr/lib/llvm-18/lib/clang/18/include/__stddef_max_align_t.h", directory: "", checksumkind: CSK_MD5, checksum: "3c0a2f19d136d39aa835c737c7105def")
!732 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !731, line: 19, size: 256, flags: DIFlagTypePassByValue, elements: !733, identifier: "_ZTS11max_align_t")
!733 = !{!734, !735}
!734 = !DIDerivedType(tag: DW_TAG_member, name: "__clang_max_align_nonce1", scope: !732, file: !731, line: 20, baseType: !298, size: 64, align: 64)
!735 = !DIDerivedType(tag: DW_TAG_member, name: "__clang_max_align_nonce2", scope: !732, file: !731, line: 22, baseType: !293, size: 128, align: 128, offset: 128)
!736 = !DIFile(filename: "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/cstddef", directory: "")
!737 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !738, file: !744, line: 82)
!738 = !DIDerivedType(tag: DW_TAG_typedef, name: "wctrans_t", file: !739, line: 48, baseType: !740)
!739 = !DIFile(filename: "/usr/include/wctype.h", directory: "", checksumkind: CSK_MD5, checksum: "eff95da6508e8f67a3c7b77d9d8ab229")
!740 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !741, size: 64)
!741 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !742)
!742 = !DIDerivedType(tag: DW_TAG_typedef, name: "__int32_t", file: !743, line: 41, baseType: !34)
!743 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/types.h", directory: "", checksumkind: CSK_MD5, checksum: "e1865d9fe29fe1b5ced550b7ba458f9e")
!744 = !DIFile(filename: "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/cwctype", directory: "")
!745 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !746, file: !744, line: 83)
!746 = !DIDerivedType(tag: DW_TAG_typedef, name: "wctype_t", file: !747, line: 38, baseType: !102)
!747 = !DIFile(filename: "/usr/include/x86_64-linux-gnu/bits/wctype-wchar.h", directory: "", checksumkind: CSK_MD5, checksum: "7f19501745f9a1fbbace8f0f185de59a")
!748 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !46, file: !744, line: 84)
!749 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !750, file: !744, line: 86)
!750 = !DISubprogram(name: "iswalnum", scope: !747, file: !747, line: 95, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!751 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !752, file: !744, line: 87)
!752 = !DISubprogram(name: "iswalpha", scope: !747, file: !747, line: 101, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!753 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !754, file: !744, line: 89)
!754 = !DISubprogram(name: "iswblank", scope: !747, file: !747, line: 146, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!755 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !756, file: !744, line: 91)
!756 = !DISubprogram(name: "iswcntrl", scope: !747, file: !747, line: 104, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!757 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !758, file: !744, line: 92)
!758 = !DISubprogram(name: "iswctype", scope: !747, file: !747, line: 159, type: !759, flags: DIFlagPrototyped, spFlags: 0)
!759 = !DISubroutineType(types: !760)
!760 = !{!34, !46, !746}
!761 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !762, file: !744, line: 93)
!762 = !DISubprogram(name: "iswdigit", scope: !747, file: !747, line: 108, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!763 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !764, file: !744, line: 94)
!764 = !DISubprogram(name: "iswgraph", scope: !747, file: !747, line: 112, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!765 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !766, file: !744, line: 95)
!766 = !DISubprogram(name: "iswlower", scope: !747, file: !747, line: 117, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!767 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !768, file: !744, line: 96)
!768 = !DISubprogram(name: "iswprint", scope: !747, file: !747, line: 120, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!769 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !770, file: !744, line: 97)
!770 = !DISubprogram(name: "iswpunct", scope: !747, file: !747, line: 125, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!771 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !772, file: !744, line: 98)
!772 = !DISubprogram(name: "iswspace", scope: !747, file: !747, line: 130, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!773 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !774, file: !744, line: 99)
!774 = !DISubprogram(name: "iswupper", scope: !747, file: !747, line: 135, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!775 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !776, file: !744, line: 100)
!776 = !DISubprogram(name: "iswxdigit", scope: !747, file: !747, line: 140, type: !252, flags: DIFlagPrototyped, spFlags: 0)
!777 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !778, file: !744, line: 101)
!778 = !DISubprogram(name: "towctrans", scope: !739, file: !739, line: 55, type: !779, flags: DIFlagPrototyped, spFlags: 0)
!779 = !DISubroutineType(types: !780)
!780 = !{!46, !46, !738}
!781 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !782, file: !744, line: 102)
!782 = !DISubprogram(name: "towlower", scope: !747, file: !747, line: 166, type: !783, flags: DIFlagPrototyped, spFlags: 0)
!783 = !DISubroutineType(types: !784)
!784 = !{!46, !46}
!785 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !786, file: !744, line: 103)
!786 = !DISubprogram(name: "towupper", scope: !747, file: !747, line: 169, type: !783, flags: DIFlagPrototyped, spFlags: 0)
!787 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !788, file: !744, line: 104)
!788 = !DISubprogram(name: "wctrans", scope: !739, file: !739, line: 52, type: !789, flags: DIFlagPrototyped, spFlags: 0)
!789 = !DISubroutineType(types: !790)
!790 = !{!738, !104}
!791 = !DIImportedEntity(tag: DW_TAG_imported_declaration, scope: !26, entity: !792, file: !744, line: 105)
!792 = !DISubprogram(name: "wctype", scope: !747, file: !747, line: 155, type: !793, flags: DIFlagPrototyped, spFlags: 0)
!793 = !DISubroutineType(types: !794)
!794 = !{!746, !104}
!795 = !{i32 7, !"Dwarf Version", i32 5}
!796 = !{i32 2, !"Debug Info Version", i32 3}
!797 = !{i32 1, !"wchar_size", i32 4}
!798 = !{i32 8, !"PIC Level", i32 2}
!799 = !{i32 7, !"PIE Level", i32 2}
!800 = !{i32 7, !"uwtable", i32 2}
!801 = !{i32 7, !"frame-pointer", i32 2}
!802 = !{!"Ubuntu clang version 18.1.3 (1ubuntu1)"}
!803 = distinct !DISubprogram(name: "main", scope: !2, file: !2, line: 16, type: !804, scopeLine: 16, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !13, retainedNodes: !806)
!804 = !DISubroutineType(types: !805)
!805 = !{!34, !34, !492}
!806 = !{}
!807 = !DILocalVariable(name: "argc", arg: 1, scope: !803, file: !2, line: 16, type: !34)
!808 = !DILocation(line: 16, column: 14, scope: !803)
!809 = !DILocalVariable(name: "argv", arg: 2, scope: !803, file: !2, line: 16, type: !492)
!810 = !DILocation(line: 16, column: 27, scope: !803)
!811 = !DILocation(line: 17, column: 22, scope: !803)
!812 = !DILocation(line: 17, column: 18, scope: !803)
!813 = !DILocation(line: 17, column: 15, scope: !803)
!814 = !DILocation(line: 18, column: 1, scope: !803)
!815 = distinct !DISubprogram(name: "foo", linkageName: "_Z3fooi", scope: !2, file: !2, line: 20, type: !343, scopeLine: 20, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !13, retainedNodes: !806)
!816 = !DILocalVariable(name: "x", arg: 1, scope: !815, file: !2, line: 20, type: !34)
!817 = !DILocation(line: 20, column: 13, scope: !815)
!818 = !DILocalVariable(name: "y", scope: !815, file: !2, line: 21, type: !34)
!819 = !DILocation(line: 21, column: 9, scope: !815)
!820 = !DILocation(line: 25, column: 17, scope: !821)
!821 = distinct !DILexicalBlock(scope: !815, file: !2, line: 24, column: 5)
!822 = !DILocation(line: 25, column: 13, scope: !821)
!823 = !DILocation(line: 25, column: 11, scope: !821)
!824 = !DILocation(line: 26, column: 5, scope: !821)
!825 = !DILocation(line: 33, column: 1, scope: !821)
!826 = !DILocalVariable(name: "e", scope: !815, file: !2, line: 27, type: !34)
!827 = !DILocation(line: 27, column: 15, scope: !815)
!828 = !DILocation(line: 29, column: 13, scope: !829)
!829 = distinct !DILexicalBlock(scope: !815, file: !2, line: 28, column: 5)
!830 = !DILocation(line: 29, column: 11, scope: !829)
!831 = !DILocation(line: 30, column: 5, scope: !829)
!832 = !DILocation(line: 31, column: 12, scope: !815)
!833 = !DILocation(line: 31, column: 5, scope: !815)
!834 = distinct !DISubprogram(name: "bar", linkageName: "_Z3bari", scope: !2, file: !2, line: 34, type: !343, scopeLine: 35, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !13, retainedNodes: !806)
!835 = !DILocalVariable(name: "x", arg: 1, scope: !834, file: !2, line: 34, type: !34)
!836 = !DILocation(line: 34, column: 13, scope: !834)
!837 = !DILocalVariable(name: "a", scope: !834, file: !2, line: 36, type: !16)
!838 = !DILocation(line: 36, column: 10, scope: !834)
!839 = !DILocalVariable(name: "b", scope: !834, file: !2, line: 36, type: !16)
!840 = !DILocation(line: 36, column: 13, scope: !834)
!841 = !DILocation(line: 39, column: 9, scope: !842)
!842 = distinct !DILexicalBlock(scope: !834, file: !2, line: 39, column: 9)
!843 = !DILocation(line: 39, column: 10, scope: !842)
!844 = !DILocation(line: 39, column: 9, scope: !834)
!845 = !DILocation(line: 41, column: 9, scope: !846)
!846 = distinct !DILexicalBlock(scope: !842, file: !2, line: 40, column: 5)
!847 = !DILocation(line: 48, column: 1, scope: !834)
!848 = !DILocation(line: 48, column: 1, scope: !846)
!849 = !DILocation(line: 43, column: 9, scope: !850)
!850 = distinct !DILexicalBlock(scope: !834, file: !2, line: 43, column: 9)
!851 = !DILocation(line: 43, column: 10, scope: !850)
!852 = !DILocation(line: 43, column: 9, scope: !834)
!853 = !DILocation(line: 45, column: 9, scope: !854)
!854 = distinct !DILexicalBlock(scope: !850, file: !2, line: 44, column: 5)
!855 = !DILocation(line: 47, column: 12, scope: !834)
!856 = distinct !DISubprogram(name: "test", linkageName: "_ZN4testC2Ev", scope: !16, file: !2, line: 8, type: !19, scopeLine: 8, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !13, declaration: !18, retainedNodes: !806)
!857 = !DILocalVariable(name: "this", arg: 1, scope: !856, type: !858, flags: DIFlagArtificial | DIFlagObjectPointer)
!858 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !16, size: 64)
!859 = !DILocation(line: 0, scope: !856)
!860 = !DILocation(line: 9, column: 23, scope: !861)
!861 = distinct !DILexicalBlock(scope: !856, file: !2, line: 8, column: 15)
!862 = !DILocation(line: 10, column: 9, scope: !856)
!863 = distinct !DISubprogram(name: "~test", linkageName: "_ZN4testD2Ev", scope: !16, file: !2, line: 11, type: !19, scopeLine: 12, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !13, declaration: !22, retainedNodes: !806)
!864 = !DILocalVariable(name: "this", arg: 1, scope: !863, type: !858, flags: DIFlagArtificial | DIFlagObjectPointer)
!865 = !DILocation(line: 0, scope: !863)
!866 = !DILocation(line: 13, column: 23, scope: !867)
!867 = distinct !DILexicalBlock(scope: !863, file: !2, line: 12, column: 9)
!868 = !DILocation(line: 14, column: 9, scope: !863)
