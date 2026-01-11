癤// Fill out your copyright notice in the Description page of Project Settings.


#include "GunWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"

AGunWeapon::AGunWeapon()
{
	WeaponType = EQPWeaponType::EWT_Gun; //臾닿린  珥湲곕 ㅼ
}

void AGunWeapon::StartFire_Implementation() //諛  ⑥ ъ
{
	if (bAutomatic) { // 諛 紐⑤ 寃쎌
		FireOnce(); // 踰 諛
		if (UWorld* World = GetWorld()) //媛 ⑦吏 
		{
			World->GetTimerManager().SetTimer(TimerHandle_AutoFire, this, &AGunWeapon::FireOnce, FireRate, true); //대㉧ ㅼ 쇱 媛寃⑹쇰 諛
		}
	}
	else {
		FireOnce(); // 諛 紐⑤媛  寃쎌  踰 諛
	}
}

void AGunWeapon::StopAttack_Implementation() //怨듦꺽 以吏 ⑥ ъ
{
	if(UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle_AutoFire); // 諛 대㉧ 댁
	}
}

// 肄 ㅼ 濡�몄 留ㅼ댁 珥 媛寃 � щ 몄ㅼ
void AGunWeapon::FireOnce() // 踰 諛 ⑥
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()); //臾닿린 瑜 罹由�곕 罹ㅽ
	if (!OwnerCharacter) return; //媛 ⑦吏 쇰㈃ 諛

	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->GetController()); //罹由�곗 而⑦몃·щ� �댁 而⑦몃·щ 罹ㅽ
	if (!PlayerController || !PlayerController->PlayerCameraManager) return; //�댁 而⑦몃·щ 移대 留ㅻ�媛 ⑦吏 쇰㈃ 諛

	const FVector Start = PlayerController->PlayerCameraManager->GetCameraLocation(); //移대 移瑜  吏�쇰 ㅼ
	const FVector Dir = PlayerController->PlayerCameraManager->GetCameraRotation().Vector(); //移대 � 諛⑺μ 諛 諛⑺μ쇰 ㅼ
	const FVector End = Start + (Dir * Range); //ш굅由щ ⑥댁 吏�  吏�쇰 ㅼ
	FCollisionQueryParams Params(SCENE_QUERY_STAT(GunFire), false); //異⑸ 荑쇰━ 쇰� ㅼ
	Params.AddIgnoredActor(this); //湲  臾댁
	Params.AddIgnoredActor(OwnerCharacter); // 臾댁
	FHitResult Hit; // 寃곌낵 蹂
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params); //쇱 몃댁 
	const FVector TraceEnd = bHit ? Hit.ImpactPoint : End; // щ 곕 몃댁  吏� ㅼ
	DrawDebugLine(GetWorld(), Start, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f); //踰洹 쇱 洹몃━湲
	
	if(bHit && Hit.GetActor()) //명쇰ŉ 명 ≫곌 ⑦ 寃쎌
	{
		
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(), BaseDamage, Dir, Hit, OwnerCharacter->GetInstigatorController(), this, DamageTypeClass);//곕�吏 �
		
	}
}
