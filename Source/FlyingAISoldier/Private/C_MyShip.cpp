// Fill out your copyright notice in the Description page of Project Settings.


#include "C_MyShip.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/InputComponent.h"
#include "C_MyCharacter.h"

// Sets default values
AC_MyShip::AC_MyShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

  //Physics section in blueprints
  MeshComp->SetSimulatePhysics(true);
  MeshComp->SetMassOverrideInKg(NAME_None, 100000.0f);
  MeshComp->bReplicatePhysicsToAutonomousProxy = true;
  MeshComp->bApplyImpulseOnDamage = true;
  MeshComp->SetEnableGravity(false);
  MeshComp->SetLinearDamping(1.0f);
  MeshComp->SetAngularDamping(1.0f);

  //Collision section in blueprints
  MeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
  MeshComp->CanCharacterStepUpOn = ECB_Yes;

  RootComponent = MeshComp;




  TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
  //FVector MeshCompSize = MeshComp->GetStaticMesh()->GetBoundingBox().GetSize();
  TriggerCapsule->InitCapsuleSize(500.0f, 300.0f);

  TriggerCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  TriggerCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  TriggerCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
  TriggerCapsule->SetupAttachment(RootComponent);
  
  TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AC_MyShip::OnOverlapBegin);
  TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AC_MyShip::OnOverlapEnd);




  DidAlreadyOverlap = false;

}

// Called when the game starts or when spawned
void AC_MyShip::BeginPlay()
{
  UE_LOG(LogTemp, Warning, TEXT("Ship Begin play called"));
	Super::BeginPlay();

}

void AC_MyShip::ExitShip()
{
  DidAlreadyOverlap = true;


  CurrentPilot->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, EDetachmentRule::KeepWorld, false));
  //CurrentPilot->AttachToComponent(CurrentPilot->GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

  APlayerController* ShipController = Cast<APlayerController>(GetController());
  ShipController->Possess(CurrentPilot);
  CurrentPilot->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
  CurrentPilot->GetCapsuleComponent()->SetEnableGravity(true);
  CurrentPilot->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
  CurrentPilot->GetCapsuleComponent()->CanCharacterStepUpOn = ECB_Yes;
  CurrentPilot->GetCapsuleComponent()->SetGenerateOverlapEvents(true);

  //CurrentPilot->GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
  //CurrentPilot->GetMesh()->CanCharacterStepUpOn = ECB_Owner;
  //CurrentPilot->GetMesh()->SetEnableGravity(true);
  MeshComp->SetEnableGravity(true);
  //APlayerController* CurrentPilotController = Cast<APlayerController>(CurrentPilot->GetController());
  //CurrentPilotController->Possess(CurrentPilot);


  //CurrentPilot->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  //TriggerCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  //CurrentPilot->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  //GetMovementComponent()->StopMovementImmediately();

  GetWorldTimerManager().SetTimer(TimerHandle_FlipDidAlreadyOverlap, this, &AC_MyShip::FlipDidAlreadyOverlap, 1.0f, false, 3.0f);

}

void AC_MyShip::FlipDidAlreadyOverlap()
{
  DidAlreadyOverlap = false;
}

// Called every frame
void AC_MyShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AC_MyShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  UE_LOG(LogTemp, Warning, TEXT("Ship SetupPlayerInputComponent called"));
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  PlayerInputComponent->BindAction("ExitVehicle", IE_Pressed, this, &AC_MyShip::ExitShip);
}


void AC_MyShip::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

  UE_LOG(LogTemp, Warning, TEXT("AC_MyShip OnOverlapBegin called"));
  MeshComp->SetEnableGravity(false);
  if (DidAlreadyOverlap) {
    return;
  }

  
  // check if Actors do not equal nullptr
  if (OtherActor && (OtherActor != this))
  {
    AC_MyCharacter* MyCharacter = Cast<AC_MyCharacter>(OtherActor);
    MyCharacter->SetVehicleInRange(this);
  }

  DidAlreadyOverlap = true;
}

void AC_MyShip::Enter(AC_MyCharacter* Pilot)
{

  Pilot->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "PilotSeat");
  APlayerController* MyCharacterController = Cast<APlayerController>(Pilot->GetController());
  MyCharacterController->Possess(this);

  //MyCharacter->GetMovementComponent()->StopMovementImmediately();
  Pilot->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  //TriggerCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  //MyCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  //APlayerController* ShipController = Cast<APlayerController>(GetController());
  //ShipController->Possess(MyCharacter);
  //UE_LOG(LogTemp, Warning, TEXT("We Began"));
  CurrentPilot = Pilot;
}

void AC_MyShip::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

  UE_LOG(LogTemp, Warning, TEXT("AC_MyShip OnOverlapEnd called"));
  if (OtherActor && (OtherActor != this))
  {
    UE_LOG(LogTemp, Warning, TEXT("We Ended"));
  }

  DidAlreadyOverlap = false;
}


