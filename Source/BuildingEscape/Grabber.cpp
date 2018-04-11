// CopyRight Ryuu Sakurai 2018

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FString objectName = GetOwner()->GetName();
	FString objectPos = GetOwner()->GetActorLocation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *objectName, *objectPos);	

	physickHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	FindComponent();
}

void UGrabber::FindComponent()
{
	if (physickHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("missing component %s: "), *(GetOwner()->GetName()));
	}

	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Relase);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("missing component %s: "), *(GetOwner()->GetName()));
	}
}

void UGrabber::Grab()
{
	auto body = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = body.GetComponent();

	if (body.GetActor() != nullptr)
	{
		physickHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}
}

void UGrabber::Relase()
{
	physickHandle->ReleaseComponent();
}
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult Hit;

	FCollisionQueryParams trace(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		trace
	);
	return Hit;
}


void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (physickHandle->GrabbedComponent)
	{
		physickHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FVector UGrabber::GetReachLineEnd()
{
	FVector playerViewPoint;
	FRotator playerRotate;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPoint,
		OUT playerRotate
	);
	return playerViewPoint + (playerRotate.Vector() * reach);
}

FVector UGrabber::GetLineStart()
{
	FVector playerViewPoint;
	FRotator playerRotate;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPoint,
		OUT playerRotate
	);
	return playerViewPoint;
}