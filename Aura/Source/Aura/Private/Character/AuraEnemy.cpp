// Copyright Mattman


#include "Character/AuraEnemy.h"
#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	if (USkeletalMeshComponent* MeshComponent = GetMesh())
		MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
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
