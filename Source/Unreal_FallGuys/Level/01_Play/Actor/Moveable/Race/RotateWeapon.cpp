// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/01_Play/Actor/Moveable/Race/RotateWeapon.h"

// Sets default values
ARotateWeapon::ARotateWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OperateMesh();
}

// Called when the game starts or when spawned
void ARotateWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	SetWeaponMesh();

	MovementComponent->LimitAngle = FRotator({ 0, 0, 60 });
}

// Called every frame
void ARotateWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovementComponent->SpinCycle(DeltaTime, Weapon, EMoveAxis::ROLL);
}

void ARotateWeapon::OperateMesh()
{
	// ActorComponent
	MovementComponent = CreateDefaultSubobject<UMovementActorComponent>(FName("MovementComponent"));

	// SetMesh And Location
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	Axis = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateAxis"));
	Axis->SetupAttachment(RootScene);
	Axis->SetRelativeLocation({ 0, 0, 0 });

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotateWeapon"));
	Weapon->SetupAttachment(Axis);
	Weapon->SetRelativeLocation({ 0, 0, 0 });

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	CapsuleCollision->SetCapsuleSize(80.0f, 125.0f);
	CapsuleCollision->SetupAttachment(Weapon);
	CapsuleCollision->SetRelativeLocation({0.0f, 0.0f, -395.0f});
	CapsuleCollision->SetRelativeRotation({0.0f, 0.0f, 90.0f});
	CapsuleCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ARotateWeapon::SetWeaponMesh_H()
{
	Axis->SetStaticMesh(UFallGlobal::GetResourceMesh("RotateWeapon_Hammer_Axis"));

	Weapon->SetStaticMesh(UFallGlobal::GetResourceMesh("RotateWeapon_Hammer_Weapon"));
}

void ARotateWeapon::SetWeaponMesh_A()
{
	Axis->SetStaticMesh(UFallGlobal::GetResourceMesh("RotateWeapon_Axe_Axis"));

	Weapon->SetStaticMesh(UFallGlobal::GetResourceMesh("RotateWeapon_Axe_Weapon"));
}

void ARotateWeapon::SetWeaponMesh()
{
	switch (WeaponType)
	{
	case EWeaponType::HAMMER:
		SetWeaponMesh_H();
		break;
	case EWeaponType::AXE:
		SetWeaponMesh_A();
		break;
	}
}

