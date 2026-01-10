#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "QPCharacter.generated.h"

UCLASS()
class PJ_QUIET_PROTOCOL_API AQPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AQPCharacter();

protected:
	virtual void BeginPlay() override; 
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override; //앉기 시작시 호출
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override; //일어서기 시작시 호출
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FORCEINLINE FVector GetDesiredCameraOffset() const { return bIsCrouched ? CrouchedCameraOffset : StandingCameraOffset; } //원하는 카메라 오프셋 반환

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Camera")
	class USpringArmComponent* CameraBoom; //3인칭 플레이를 위한 스프링암 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera; //플레이어를 따라다니는 카메라

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

private:
	void UpdateMovementSpeed(); //움직임 속도 업데이트
	bool bWantsToSprint = false; //달리기 의사 여부

};
