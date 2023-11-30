/*
* Copyright (c) <2021> Side Effects Software Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*
* 2. The name of Side Effects Software may not be used to endorse or
*    promote products derived from this software without specific prior
*    written permission.
*
* THIS SOFTWARE IS PROVIDED BY SIDE EFFECTS SOFTWARE "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
* NO EVENT SHALL SIDE EFFECTS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "HAPI/HAPI_Common.h"

#include "HoudiniGeoPartObject.h"
#include "HoudiniOutput.h"
#include "HoudiniPackageParams.h"
//#include "HoudiniAssetComponent.h"
//#include "HoudiniMaterialTranslator.h"

#include "CoreMinimal.h"
//#include "UObject/ObjectMacros.h"
#include "ImportUtils/SkeletalMeshImportUtils.h"
#include "Rendering/SkeletalMeshLODImporterData.h"

class USkeletalMesh;
class USkeleton;


struct FHoudiniSkeletalMeshParts
{
    const FHoudiniGeoPartObject* HGPOShapeInstancer = nullptr;
	const FHoudiniGeoPartObject* HGPOShapeMesh = nullptr;
	const FHoudiniGeoPartObject* HGPOPoseInstancer = nullptr;
	const FHoudiniGeoPartObject* HGPOPoseMesh = nullptr;

    const FHoudiniGeoPartObject* GetMainHGPO() const { return HGPOShapeInstancer; }

    bool IsValid() const { return HGPOShapeInstancer && HGPOShapeMesh && HGPOPoseInstancer && HGPOPoseMesh; }
};


struct SKBuildSettings
{
    FSkeletalMeshImportData SkeletalMeshImportData;
    bool bIsNewSkeleton = false;
    float ImportScale = 1.0f;
    USkeletalMesh* SKMesh = nullptr;
    UPackage* SKPackage = nullptr;
    USkeleton* Skeleton = nullptr;
    FString CurrentObjectName;
    // HAPI_NodeId GeoId = -1;
    // HAPI_NodeId PartId = -1;
    FHoudiniSkeletalMeshParts SKParts;
    bool ImportNormals = false;
    bool OverwriteSkeleton = false;
    FString SkeletonAssetPath = "";
    int NumTexCoords = 1;
};


struct HOUDINIENGINE_API FHoudiniSkeletalMeshTranslator
{
    public:
    
        // Check whether the packed primitive is skeleton Rest Geometry
        static bool IsRestGeometryInstancer(const HAPI_NodeId& GeoId, const HAPI_NodeId& PartId, FString& OutBaseName);
        static bool IsRestGeometryMesh(const HAPI_NodeId& GeoId, const HAPI_NodeId& PartId);

        // Check whether the packed primitive is skeleton Rest Geometry
        static bool IsCapturePoseInstancer(const HAPI_NodeId& GeoId, const HAPI_NodeId& PartId, FString& OutBaseName);
        static bool IsCapturePoseMesh(const HAPI_NodeId& GeoId, const HAPI_NodeId& PartId);

protected:
        // Helper to IsRestGeometry* / IsCapturePose* functions
        static HAPI_AttributeInfo GetAttrInfo(const HAPI_NodeId& GeoId, const HAPI_NodeId& PartId, const char* AttrName, HAPI_AttributeOwner AttrOwner);

public:
        static bool CreateAllSkeletalMeshesAndComponentsFromHoudiniOutput(
            UHoudiniOutput* InOutput,
            const FHoudiniPackageParams& InPackageParams,
            const TMap<FHoudiniMaterialIdentifier, UMaterialInterface*>& InAllOutputMaterials,
            UObject* InOuterComponent);

        //
        static bool CreateSkeletalMeshFromHoudiniGeoPartObject(
            const FHoudiniSkeletalMeshParts& SKParts,
            const FHoudiniPackageParams& InPackageParams,
            UObject* InOuterComponent,
            const TMap<FHoudiniOutputObjectIdentifier, FHoudiniOutputObject>&
            InOutputObjects, TMap<FHoudiniOutputObjectIdentifier, FHoudiniOutputObject>&
            OutOutputObjects);

        bool CreateSkeletalMesh_SkeletalMeshImportData();        

        static void ExportSkeletalMeshAssets(UHoudiniOutput* InOutput);
        // static bool HasSkeletalMeshData(const HAPI_NodeId& GeoId, const HAPI_NodeId& PartId);
        static void LoadImportData(const HAPI_NodeId& GeoId, const HAPI_NodeId& PartId);
        static void CreateSKAssetAndPackage(SKBuildSettings& BuildSettings, const FHoudiniSkeletalMeshParts& InSKParts, FString PackageName, int MaxInfluences = 1, bool ImportNormals = false);
        static void BuildSKFromImportData(SKBuildSettings& BuildSettings, TArray<FSkeletalMaterial>& Materials);
        static bool SKImportData(SKBuildSettings& BuildSettings);
        static void UpdateBuildSettings(SKBuildSettings& BuildSettings);

        static bool FindAttributeOnSkeletalMeshShapeParts(const FHoudiniSkeletalMeshParts& InSKParts, const char* Attribute, HAPI_NodeId& OutGeoId, HAPI_PartId& OutPartId);

        //-----------------------------------------------------------------------------------------------------------------------------
        // MUTATORS
        //-----------------------------------------------------------------------------------------------------------------------------
        void SetHoudiniSkeletalMeshParts(const FHoudiniSkeletalMeshParts& InSKParts) { SKParts = InSKParts; };
        void SetPackageParams(const FHoudiniPackageParams& InPackageParams, const bool& bUpdateHGPO = false);
        void SetInputObjects(const TMap<FHoudiniOutputObjectIdentifier, FHoudiniOutputObject>& InInputObjects) { InputObjects = InInputObjects; };
        void SetOutputObjects(const TMap<FHoudiniOutputObjectIdentifier, FHoudiniOutputObject>& InOutputObjects) { OutputObjects = InOutputObjects; };
        void SetOuterComponent(UObject* InOuterComponent) { OuterComponent = InOuterComponent; }

        // Current / Previous Output objects
        TMap<FHoudiniOutputObjectIdentifier, FHoudiniOutputObject> InputObjects;

        // New Output objects
        TMap<FHoudiniOutputObjectIdentifier, FHoudiniOutputObject> OutputObjects;

    protected:

        // The HoudiniGeoPartObjects we're working on
        FHoudiniSkeletalMeshParts SKParts;
        // Structure that handles cooking/baking package creation parameters
        FHoudiniPackageParams PackageParams;
        
        // New Output objects
        //TMap<FHoudiniOutputObjectIdentifier, FHoudiniOutputObject> OutputObjects;

        // Outer object for attaching components to
        UObject* OuterComponent = nullptr;
        

        USkeletalMesh* CreateNewSkeletalMesh(const FString& InSplitIdentifier);
        USkeleton* CreateNewSkeleton(const FString& InSplitIdentifier) const;
};