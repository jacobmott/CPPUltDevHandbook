// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Enemy.generated.h"

UCLASS()
class ULT_DEV_HNDBK_API AEnemy : public ABaseCharacter
{
  GENERATED_BODY()

public:
  // Sets default values for this character's properties
  AEnemy();

  UPROPERTY(EditAnywhere, Category = "Pawn")
  class UBehaviorTree* BehaviorTree;

  UPROPERTY(EditAnywhere, Category = "BehaviorTree", Meta = (MakeEditWidget = true))
  FVector PatrolPoint1;

  UPROPERTY(EditAnywhere, Category = "BehaviorTree", Meta = (MakeEditWidget = true))
  FVector PatrolPoint2;

  UPROPERTY(EditAnywhere, Category = "BehaviorTree", Meta = (MakeEditWidget = true))
  FVector PatrolPoint3;

  UPROPERTY(EditAnywhere, Category = "BehaviorTree", Meta = (MakeEditWidget = true))
  FVector PatrolPoint4;

  class AEnemyController* EnemyController;


  UPROPERTY(VisibleAnywhere, Category = "BehaviorTree")
  class USphereComponent* AgroSphere;

  UPROPERTY(VisibleAnywhere, Category = "BehaviorTree")
  USphereComponent * AttackSphere;


  UFUNCTION()
  void AgroSphereBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

  UFUNCTION()
  void AgroSphereEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);


  UFUNCTION()
  void AttackSphereBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

  UFUNCTION()
  void AttackSphereEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);


protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void StartAttack();
  void Attack();

public: 
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

  UPROPERTY(BlueprintReadOnly)
  bool bIsAccelerating;

  UFUNCTION(BlueprintCallable)
  void SetIsAccelerating(bool bAccelerating); 

  bool bAttackTimerStarted;
  bool bInAttackRange;


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
  float AttackMinTime;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
  float AttackMaxTime;

  FTimerHandle AttackTimer;

  virtual void SwordBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
  virtual void Die(AActor* Causer) override;
  virtual void DeathEnd() override;

};
