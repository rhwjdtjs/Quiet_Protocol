// Fill out your copyright notice in the Description page of Project Settings.


#include "GunWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

AGunWeapon::AGunWeapon()
{
	WeaponType = EQPWeaponType::EWT_Gun; //무기 타입을 총기로 설정
}

void AGunWeapon::StartFire_Implementation() //발사 시작 함수 재정의
{
	if (bAutomatic) { //자동 발사 모드인 경우
		FireOnce(); //한 번 발사
		if (UWorld* World = GetWorld()) //월드가 유효한지 확인
		{
			World->GetTimerManager().SetTimer(TimerHandle_AutoFire, this, &AGunWeapon::FireOnce, FireRate, true); //타이머 설정하여 일정 간격으로 발사
		}
	}
	else {
		FireOnce(); //자동 발사 모드가 아닌 경우 한 번 발사
	}
}

void AGunWeapon::StopAttack_Implementation() //공격 중지 함수 재정의
{
	if(UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_AutoFire); //자동 발사 타이머 해제
	}
}

//임시 코드 다음에 프로젝트일 만들어서 총알 나가게 수정 현재는 히트스캔
void AGunWeapon::FireOnce() //한 번 발사 함수
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()); //무기 소유자를 캐릭터로 캐스팅
	if (!OwnerCharacter) return; //소유자가 유효하지 않으면 반환

	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()); //캐릭터의 컨트롤러를 플레이어 컨트롤러로 캐스팅
	if (!PlayerController || !PlayerController->PlayerCameraManager) return; //플레이어 컨트롤러나 카메라 매니저가 유효하지 않으면 반환

	const FVector Start = PlayerController->PlayerCameraManager->GetCameraLocation(); //카메라 위치를 시작 지점으로 설정
	const FVector Dir = PlayerController->PlayerCameraManager->GetCameraRotation().Vector(); //카메라 회전 방향을 발사 방향으로 설정
	const FVector End = Start + (Dir * Range); //사거리만큼 떨어진 지점을 끝 지점으로 설정
	FCollisionQueryParams Params(SCENE_QUERY_STAT(GunFire), false); //충돌 쿼리 파라미터 설정
	Params.AddIgnoredActor(this); //자기 자신 무시
	Params.AddIgnoredActor(OwnerCharacter); //소유자 무시
	FHitResult Hit; //히트 결과 변수
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params); //라인 트레이스 수행
	const FVector TraceEnd = bHit ? Hit.ImpactPoint : End; //히트 여부에 따라 트레이스 끝 지점 설정
	DrawDebugLine(GetWorld(), Start, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f); //디버그 라인 그리기
	if(bHit && Hit.GetActor()) //히트했으며 히트한 액터가 유효한 경우
	{
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), BaseDamage, Dir, Hit, OwnerCharacter->GetInstigatorController(), this, DamageTypeClass); //데미지 적용
	}
}
