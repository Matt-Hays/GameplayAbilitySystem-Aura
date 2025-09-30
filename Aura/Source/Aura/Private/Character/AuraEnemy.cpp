// Copyright Mattman

#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	if (USkeletalMeshComponent* MeshComponent = GetMesh())
		MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); // Server replication

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void AAuraEnemy::HighlightActor()
{
	if (USkeletalMeshComponent* MeshComponent = GetMesh())
	{
		MeshComponent->SetRenderCustomDepth(true);
		if (MeshComponent->CustomDepthStencilValue != CUSTOM_DEPTH_RED)
			MeshComponent->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}

	if (IsValid(Weapon))
	{
		Weapon->SetRenderCustomDepth(true);
		if (Weapon->CustomDepthStencilValue != CUSTOM_DEPTH_RED)
			Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AAuraEnemy::UnHighlightActor()
{
	if (USkeletalMeshComponent* MeshComponent = GetMesh())
		MeshComponent->SetRenderCustomDepth(false);
	if (IsValid(Weapon))
		Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
