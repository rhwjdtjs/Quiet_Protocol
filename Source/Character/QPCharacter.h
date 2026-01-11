#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PJ_Quiet_Protocol/Commons/QPCombatTypes.h"
#include "QPCharacter.generated.h"

class UQPCombatComponent; //전방 선언
class AWeaponBase; //전방 선언
UCLASS()
class PJ_QUIET_PROTOCOL_API AQPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AQPCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintPure, Category = "Combat")
	FORCEINLINE FVector GetDesiredCameraOffset() const { return bIsCrouched ? CrouchedCameraOffset : StandingCameraOffset; } //원하는 카메라 오프셋 반환
	UFUNCTION(BlueprintPure, Category = "Combat")
	FORCEINLINE UQPCombatComponent* GetCombatComponent() const { return CombatComponent; } //전투 컴포넌트 반환 함수
	UFUNCTION(BlueprintPure, Category = "Combat")
	FORCEINLINE EQPWeaponType GetWeaponType() const { return Weapontype; } //장착된 무기 타입 반환 함수
	UFUNCTION(BlueprintPure, Category = "Combat")
	FORCEINLINE bool IsSprinting() const { return bWantsToSprint && !bIsCrouched; }
protected:
	virtual void BeginPlay() override; 
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override; //앉기 시작시 호출
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override; //일어서기 시작시 호출

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UQPCombatComponent* CombatComponent; //전투 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera")
	class USpringArmComponent* CameraBoom; //3인칭 플레이를 위한 스프링암 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera; //플레이어를 따라다니는 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	EQPWeaponType Weapontype = EQPWeaponType::EWT_None; //장착된 무기 타입
	UFUNCTION()
	void HandleWeaponTypeChanged(EQPWeaponType NewWeaponType); //무기 타입 변경 핸들러
	//움직임 속도 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 600.f; //걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 900.f; //달리기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeed = 300.f; //앉기 속도

	//앉기 카메라 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Crouch")
	FVector StandingCameraOffset = FVector::ZeroVector; //서있을 때 카메라 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Crouch")
	FVector CrouchedCameraOffset = FVector(0.f, 0.f, -40.f); //앉아있을 때 카메라 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Crouch", meta = (ClampMin = "0.0")) 
	float CrouchCameraInterpSpeed = 12.f; //카메라 위치 보간 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (ClampMin = "0.0")) 
	float CameraArmLength = 300.f; //카메라와 캐릭터 사이 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon", meta = (ClampMin= "0.0"))
	float EquipTraceDistance = 250.f; //무기 장착 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon")
	bool bDrawEquipTraceDebug = false; //무기 장착 거리 디버그 선 그리기 여부
	//입력 함수들
	void MoveForward(float Value); //앞뒤 이동
	void MoveRight(float Value); //좌우 이동
	void Turn(float Value); //좌우 회전
	void LookUp(float Value); //상하 회전
	void StartJump(); //점프 시작
	void StopJump(); //점프 멈춤
	void ToggleCrouch(); //앉기/일어서기 토글
	void StartSprint(); //달리기 시작
	void StopSprint(); //달리기 멈춤
	void TryEquipWeapon(); //무기 장착 시도 함수
	void AttackPressed(); //공격 버튼 눌림
	void AttackReleased(); //공격 버튼 떼짐

private:
	void UpdateMovementSpeed(); //움직임 속도 업데이트
	bool bWantsToSprint = false; //달리기 의사 여부

};
