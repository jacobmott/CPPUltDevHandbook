// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "MainPlayerController.h"
#include "CountessSaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  // Don't rotate when the Controller rotates. Let that just affect the camera.
  bUseControllerRotationPitch = false;
  bUseControllerRotationYaw = false;
  bUseControllerRotationRoll = false;

  // Configure character movement
  GetCharacterMovement()->bOrientRotationToMovement = true; 
  // Character moves in the direction of input... 
  // ...at this rotation rate
  GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); 

  // Create a camera boom (pulls in towards the player if there is a collision)
  CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
  CameraBoom->SetupAttachment(RootComponent);

  // The camera follows at this distance behind the character
  CameraBoom->TargetArmLength = 300.0f; 
  
  // Rotate the arm based on the Controller
  CameraBoom->bUsePawnControlRotation = true;

  // Create a follow camera
  FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
  // Attach the camera to the end of the boom and let the boom adjust to match the Controller orientation
  FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

  // Camera does not rotate relative to arm
  FollowCamera->bUsePawnControlRotation = false;

  bAttacking = false;

  Health = 85.f;
  MaxHealth = 100.f;

}

void AMainCharacter::SetHealth(float Amount) { 
  if (Amount > MaxHealth) { 
    Health = MaxHealth;
  }
  else if (Amount < 0) { 
    Health = 0; 
  }
  else { 
    Health = Amount; 
  } 
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
  Super::BeginPlay();
  MainPlayerController = Cast<AMainPlayerController>(GetController());
  
}

void AMainCharacter::MoveForward(float Value)
{
  if ((Controller != NULL) && (Value != 0.0f)) {
    // find out which way is forward
    const FRotator Rotation = Controller->GetControlRotation();
    /** Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down) */
    //float Pitch;
    /** Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South. */
    //float Yaw;
    /** Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW. */
    //float Roll;
    //FORCEINLINE FRotator(float InPitch, float InYaw, float InRoll);
    const FRotator YawRotation(0, Rotation.Yaw, 0);
    // get forward vector
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(Direction, Value);
  }

}

void AMainCharacter::MoveRight(float Value)
{
  if ((Controller != NULL) && (Value != 0.0f)) {
    // find out which way is right
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
    // get right vector
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    // add movement in that direction 
    AddMovementInput(Direction, Value);
  }

}

void AMainCharacter::LMBDown()
{
  if (bAttacking) { return; }
  bAttacking = true;
  UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
  if (AnimInstance && CountessAttackMontage) { 
    AnimInstance->Montage_Play(CountessAttackMontage);
  }
}

void AMainCharacter::ESCDown()
{
  if (MainPlayerController) {
    MainPlayerController->TogglePauseMenu(); 
  }
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);
  PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
  PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

  PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
  PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

  PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
  PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

  PlayerInputComponent->BindAction("LMBDown", IE_Pressed, this, &AMainCharacter::LMBDown);

  PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMainCharacter::ESCDown);

}

void AMainCharacter::SaveGame() {
  UCountessSaveGame * SaveGameInstance = Cast<UCountessSaveGame>(UGameplayStatics::CreateSaveGameObject(UCountessSaveGame::StaticClass()));
  SaveGameInstance->Health = Health;
  SaveGameInstance->MaxHealth = MaxHealth;
  SaveGameInstance->WorldLocation = GetActorLocation();
  SaveGameInstance->WorldRotation = GetActorRotation();
  UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserSlot);
}


void AMainCharacter::LoadGame() {
  UCountessSaveGame* LoadGameInstance = Cast<UCountessSaveGame>(UGameplayStatics::CreateSaveGameObject(UCountessSaveGame::StaticClass()));
  LoadGameInstance = Cast<UCountessSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserSlot));
  Health = LoadGameInstance->Health;
  MaxHealth = LoadGameInstance->MaxHealth;
  SetActorLocation(LoadGameInstance->WorldLocation);
  SetActorRotation(LoadGameInstance->WorldRotation);
}

