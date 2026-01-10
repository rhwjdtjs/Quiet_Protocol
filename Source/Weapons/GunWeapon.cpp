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

void AGunWeapon::StartFire_Implementation()
{

}

void AGunWeapon::StopAttack_Implementation()
{

}

//임시 코드 다음에 프로젝트일 만들어서 총알 나가게 수정 현재는 히트스캔
void AGunWeapon::FireOnce()
{

}
