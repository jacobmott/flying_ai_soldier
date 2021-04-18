// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "C_MyShip.h"

// Sets default values
AC_MyCharacter::AC_MyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
  SpringArmComp->bUsePawnControlRotation = true;
  SpringArmComp->SetupAttachment(RootComponent);

  CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
  CameraComp->SetupAttachment(SpringArmComp);
  
}

// Called when the game starts or when spawned
void AC_MyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AC_MyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AC_MyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
  PlayerInputComponent->BindAxis("MoveForward", this, &AC_MyCharacter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &AC_MyCharacter::MoveRight);

  PlayerInputComponent->BindAxis("LookUp", this, &AC_MyCharacter::AddControllerPitchInput);
  PlayerInputComponent->BindAxis("Turn", this, &AC_MyCharacter::AddControllerYawInput);


  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AC_MyCharacter::BeginJump);
  PlayerInputComponent->BindAction("EnterVehicle", IE_Pressed, this, &AC_MyCharacter::EnterVehicle);
}


void AC_MyCharacter::MoveForward(float Value)
{
  UE_LOG(LogTemp, Warning, TEXT("AC_MyCharacter MoveForward called"));
  AddMovementInput(GetActorForwardVector() * Value);

}

void AC_MyCharacter::MoveRight(float Value)
{
  UE_LOG(LogTemp, Warning, TEXT("AC_MyCharacter MoveRight called"));
  AddMovementInput(GetActorRightVector() * Value);
}


void AC_MyCharacter::BeginJump()
{
  Jump();
}

void AC_MyCharacter::EnterVehicle()
{
  if (VehicleInRange) {
    VehicleInRange->Enter(this);
  }
}


void AC_MyCharacter::SetVehicleInRange(AC_MyShip* Ship)
{
  VehicleInRange = Ship;
}
