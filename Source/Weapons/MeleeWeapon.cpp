// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"

AMeleeWeapon::AMeleeWeapon()
{
	WeaponType = EQPWeaponType::EWT_Melee; //臾닿린  洹쇱 臾닿린濡 ㅼ
}

void AMeleeWeapon::StartFire_Implementation() //怨듦꺽  ⑥ ъ
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()); //臾닿린 瑜 罹由�곕 罹ㅽ
	if (!OwnerCharacter) return; //媛 ⑦吏 쇰㈃ 諛
	const FVector Start = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 50.f; //罹由� 履쎌쇰  吏� ㅼ
	const FVector End = Start + OwnerCharacter->GetActorForwardVector() * SwingRange; //ㅼ 諛寃쎈 ⑥댁 吏�  吏�쇰 ㅼ
	FCollisionQueryParams Params(SCENE_QUERY_STAT(MeleeSwing), false); //異⑸ 荑쇰━ 쇰� ㅼ
	Params.AddIgnoredActor(this); //湲  臾댁
	Params.AddIgnoredActor(OwnerCharacter); // 臾댁
	TArray<FHitResult> HitResults; // 寃곌낵 諛곗
	const bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SwingRadius), Params); //ㅼ 
	DrawDebugSphere(GetWorld(), End, SwingRadius, 12, FColor::Green, false, 0.35f); //踰洹 ㅽ쇱 洹몃━湲
	if (!bHit) return;	//명吏 쇰㈃ 諛
	for (const FHitResult& Hit : HitResults)// 寃곌낵 諛蹂
	{
		AActor* HitActor = Hit.GetActor(); //명 ≫ 媛�몄ㅺ린
		if (!HitActor) continue; //명 ≫곌 ⑦吏 쇰㈃ ㅼ쇰
		UGameplayStatics::ApplyPointDamage(HitActor, BaseDamage, OwnerCharacter->GetActorForwardVector(), Hit, OwnerCharacter->GetController(),
			this, DamageTypeClass); //곕�吏 �
		break; //泥 踰吏 명 ≫곗留 곕�吏 �  醫猷
	}
	
}
