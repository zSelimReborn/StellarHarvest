// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AffiliationComponent.h"

UAffiliationComponent::UAffiliationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UAffiliationComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAffiliationComponent::OnRegister()
{
	Super::OnRegister();
	ChangeAffiliation(AffiliationTag);
}

void UAffiliationComponent::OnUnregister()
{
	Super::OnUnregister();
	RemoveAffiliation();
}

bool UAffiliationComponent::IsFriendly(const AActor* TestingActor) const
{
	return TestingActor->ActorHasTag(AffiliationTag);
}

void UAffiliationComponent::ChangeAffiliation(const FName& NewAffiliation)
{
	if (GetOwner()->ActorHasTag(NewAffiliation))
	{
		return;
	}

	RemoveAffiliation();
	AffiliationTag = NewAffiliation;
	GetOwner()->Tags.Add(AffiliationTag);
}

FName UAffiliationComponent::GetAffiliation() const
{
	return AffiliationTag;
}

void UAffiliationComponent::RemoveAffiliation()
{
	GetOwner()->Tags.Remove(AffiliationTag);
}

