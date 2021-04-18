// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "C_MyShip.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class AC_MyCharacter;

UCLASS()
class FLYINGAISOLDIER_API AC_MyShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AC_MyShip();


  // declare overlap begin function
  UFUNCTION()
  void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

  // declare overlap end function
  UFUNCTION()
  void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
  UCapsuleComponent* TriggerCapsule;

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	UStaticMeshComponent* MeshComp;

	bool DidAlreadyOverlap;
	void ExitShip();
	AC_MyCharacter* CurrentPilot;
	FTimerHandle TimerHandle_FlipDidAlreadyOverlap;
	void FlipDidAlreadyOverlap();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void Enter(AC_MyCharacter* Pilot);
};
