#include "QPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "PJ_Quiet_Protocol/Character/Components/QPCombatComponent.h"
#include "PJ_Quiet_Protocol/Weapons/WeaponBase.h"

AQPCharacter::AQPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false; //컨트롤러의 Yaw 회전에 따라 캐릭터 회전 안함
	bUseControllerRotationPitch = false; //컨트롤러의 Pitch 회전에 따라 캐릭터 회전 안함
	bUseControllerRotationRoll = false; //컨트롤러의 Roll 회전에 따라 캐릭터 회전 안함

	UCharacterMovementComponent* MoveComponent = GetCharacterMovement(); //캐릭터 무브먼트 컴포넌트 가져오기
	if(ensure(MoveComponent)) //무브먼트 컴포넌트가 유효한지 확인
	{
		MoveComponent->bOrientRotationToMovement = true; //이동 방향으로 캐릭터 회전 설정
		MoveComponent->RotationRate = FRotator(0.f, 540.f, 0.f); //회전 속도 설정
		MoveComponent->GetNavAgentPropertiesRef().bCanCrouch = true; //앉기 가능 설정
		MoveComponent->MaxWalkSpeed = WalkSpeed; //기본 걷기 속도 설정
		MoveComponent->MaxWalkSpeedCrouched = CrouchSpeed; //앉기 속도 설정
	}
	//카메라 붐 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); //스프링암 컴포넌트 생성
	CameraBoom->SetupAttachment(RootComponent); //루트 컴포넌트에 부착
	CameraBoom->TargetArmLength = CameraArmLength; //카메라와 캐릭터 사이 거리 설정
	CameraBoom->bUsePawnControlRotation = true; //컨트롤러 회전에 따라 카메라 회전 설정
	CameraBoom->bEnableCameraLag = true; //카메라 지연 활성화
	CameraBoom->CameraLagSpeed = 10.f; //카메라 지연 속도 설정

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); //카메라 컴포넌트 생성
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); //카메라 붐에 부착
	FollowCamera->bUsePawnControlRotation = false; //카메라가 폰의 회전에 따라 회전하지 않도록 설정
	
	//컴뱃 컴포넌트
	CombatComponent = CreateDefaultSubobject<UQPCombatComponent>(TEXT("CombatComponent")); //전투 컴포넌트 생성
}

void AQPCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(CameraBoom)
	{
		CameraBoom->TargetArmLength = CameraArmLength; //카메라와 캐릭터 사이 거리 설정
		CameraBoom->TargetOffset = StandingCameraOffset; //서있을 때 카메라 오프셋 설정
	}
	UCharacterMovementComponent* MoveComponent = GetCharacterMovement(); //캐릭터 무브먼트 컴포넌트 가져오기
	if (MoveComponent)
	{
		MoveComponent->MaxWalkSpeedCrouched = CrouchSpeed; //앉기 속도 설정
		MoveComponent->GetNavAgentPropertiesRef().bCanCrouch = true; //앉기 가능 설정
	}

	UpdateMovementSpeed(); //움직임 속도 업데이트
}

void AQPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(CameraBoom && CrouchCameraInterpSpeed > 0.f)
	{
		const FVector DesiredOffset = GetDesiredCameraOffset(); //원하는 카메라 오프셋 계산
		const FVector NewOffset = FMath::VInterpTo(CameraBoom->TargetOffset, DesiredOffset, DeltaTime, CrouchCameraInterpSpeed); //카메라 오프셋 보간
		CameraBoom->TargetOffset = NewOffset; //카메라 붐의 타겟 오프셋 업데이트
	}
}

void AQPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent); //입력 컴포넌트 유효성 검사
	PlayerInputComponent->BindAxis("MoveForward", this, &AQPCharacter::MoveForward); //앞뒤 이동 바인딩
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AQPCharacter::MoveRight); //좌우 이동 바인딩
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AQPCharacter::Turn); //좌우 회전 바인딩
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AQPCharacter::LookUp); //상하 회전 바인딩
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AQPCharacter::StartJump); //점프 바인딩
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AQPCharacter::StopJump); //점프 멈춤 바인딩
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AQPCharacter::ToggleCrouch); //앉기/일어서기 토글 바인딩
	// Sprint (Hold)
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AQPCharacter::StartSprint); //달리기 시작 바인딩
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AQPCharacter::StopSprint); //달리기 멈춤 바인딩
	// input 추가
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AQPCharacter::AttackPressed); //발사 바인딩
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Released, this, &AQPCharacter::AttackReleased); //발사 멈춤 바인딩
	PlayerInputComponent->BindAction(TEXT("Equip"), IE_Pressed, this, &AQPCharacter::TryEquipWeapon); //장착 바인딩
}
void AQPCharacter::HandleWeaponTypeChanged(EQPWeaponType NewWeaponType)
{
	Weapontype = NewWeaponType; //장착된 무기 타입 업데이트
}
//움직임 함수들
void AQPCharacter::MoveForward(float Value) //앞뒤 이동
{
	if (!Controller || FMath::IsNearlyZero(Value)) return; //컨트롤러가 없거나 입력 값이 거의 0이면 함수 종료

	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f); //컨트롤러의 Yaw 회전 가져오기
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); //앞 방향 벡터 계산
	AddMovementInput(Direction, Value); //이동 입력 추가
}
void AQPCharacter::MoveRight(float Value)
{
	if (!Controller || FMath::IsNearlyZero(Value)) return; //컨트롤러가 없거나 입력 값이 거의 0이면 함수 종료

	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f); //컨트롤러의 Yaw 회전 가져오기
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //오른쪽 방향 벡터 계산
	AddMovementInput(Direction, Value); //이동 입력 추가
}
void AQPCharacter::Turn(float Value) { AddControllerYawInput(Value); }  //컨트롤러의 Yaw 입력 추가
void AQPCharacter::LookUp(float Value) { AddControllerPitchInput(Value); } //컨트롤러의 Pitch 입력 추가
void AQPCharacter::StartJump() { Jump(); } //점프 시작
void AQPCharacter::StopJump() { StopJumping(); } //점프 멈춤
void AQPCharacter::ToggleCrouch()
{
	UCharacterMovementComponent* MoveComponent = GetCharacterMovement(); //캐릭터 무브먼트 컴포넌트 가져오기
	if (!MoveComponent || !MoveComponent->GetNavAgentPropertiesRef().bCanCrouch) return; //무브먼트 컴포넌트가 없거나 앉기 불가능하면 함수 종료
	if (bIsCrouched)
	{
		UnCrouch(); //일어서기
	}
	else
	{
		Crouch(); //앉기
	}
}
void AQPCharacter::StartSprint()
{
	bWantsToSprint = true; //달리기 의사 설정
	UpdateMovementSpeed(); //움직임 속도 업데이트
}
void AQPCharacter::StopSprint()
{
	bWantsToSprint = false; //달리기 의사 해제
	UpdateMovementSpeed(); //움직임 속도 업데이트
}
void AQPCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust); //부모 클래스의 OnStartCrouch 호출
	UpdateMovementSpeed(); //움직임 속도 업데이트
}
void AQPCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust); //부모 클래스의 OnEndCrouch 호출
	UpdateMovementSpeed(); //움직임 속도 업데이트
}
void AQPCharacter::UpdateMovementSpeed()
{
	UCharacterMovementComponent* MoveComponent = GetCharacterMovement(); //캐릭터 무브먼트 컴포넌트 가져오기
	if (!MoveComponent) return; //무브먼트 컴포넌트가 없으면 함수 종료
	if (bIsCrouched)
	{
		MoveComponent->MaxWalkSpeed = CrouchSpeed; //앉기 속도 설정
		return; //앉아있을 때는 달리기 여부와 상관없이 앉기 속도 사용
	}
	MoveComponent->MaxWalkSpeed = bWantsToSprint ? SprintSpeed : WalkSpeed; //달리기 의사에 따라 속도 설정
}
//전투 (Combat) 관련 함수들
void AQPCharacter::TryEquipWeapon() {
	if (!CombatComponent) return;
	const FVector TraceStart = FollowCamera ? FollowCamera->GetComponentLocation() : GetActorLocation(); //카메라 위치 가져오기
	const FVector TraceDir = FollowCamera ? FollowCamera->GetForwardVector() : GetActorForwardVector(); //카메라 앞 방향 벡터 가져오기
	const FVector TraceEnd = TraceStart + (TraceDir * EquipTraceDistance); //트레이
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Equiptrace), false); //충돌 쿼리 파라미터 설정
	Params.AddIgnoredActor(this); //자기 자신 무시
	FHitResult Hit; //히트 결과 변수
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params); //라인 트레이스 수행
	if (bDrawEquipTraceDebug) { //디버그 선 그리기 설정이 켜져 있으면
		const FVector DebugEnd = bHit ? Hit.ImpactPoint : TraceEnd; //디버그 선의 끝 위치 계산
		DrawDebugLine(GetWorld(), TraceStart, DebugEnd, FColor::Yellow, false, 2.f, 0, 1.f); //디버그 선 그리기
	}
	if (!bHit) return; //아무것도 맞지 않으면 함수 종료
	if(AWeaponBase* HitWeapon = Cast<AWeaponBase>(Hit.GetActor())) //맞은 액터가 무기인지 확인
	{
		CombatComponent->EquipWeapon(HitWeapon, true); //무기 장착 시도
	}
}
void AQPCharacter::AttackPressed()
{
	if (CombatComponent) CombatComponent->StartAttack(); //공격 시작
}
void AQPCharacter::AttackReleased()
{
	if (CombatComponent) CombatComponent->StopAttack(); //공격 멈춤
}
