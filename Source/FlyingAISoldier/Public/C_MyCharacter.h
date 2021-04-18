// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C_MyCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputComponent;
class AC_MyShip;

UCLASS()
class FLYINGAISOLDIER_API AC_MyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_MyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  void MoveForward(float Value);
  void MoveRight(float Value);

  void BeginJump();


  void EnterVehicle();
 	AC_MyShip* VehicleInRange;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
  UCameraComponent* CameraComp;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components");
  USpringArmComponent* SpringArmComp;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  void SetVehicleInRange(AC_MyShip* Ship);

};
