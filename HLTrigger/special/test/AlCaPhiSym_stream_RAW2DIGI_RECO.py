# Auto generated configuration file
# using:
# Revision: 1.381.2.13
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v
# with command line options: data -s RAW2DIGI,L1Reco,RECO --eventcontent RECO --conditions auto:com10 -n 500 --no_exec --data --scenario pp --process RERECO
import FWCore.ParameterSet.Config as cms

process = cms.Process('PhiSymStream')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
#process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
#process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('RecoLocalCalo.Configuration.ecalLocalRecoSequence_cff')


process.maxEvents = cms.untracked.PSet(
                                       input = cms.untracked.int32(100)
                                       )

# Input source
process.source = cms.Source("PoolSource",
                            secondaryFileNames = cms.untracked.vstring(),
                            fileNames = cms.untracked.vstring(
                                                         
                                                                        'root://eoscms//eos/cms/store/data/Run2012A/MinimumBias/RAW/v1/000/191/226/6C656CC9-0C86-E111-8945-003048F118C2.root',
                                                                        #'file:/afs/cern.ch/work/b/bmarzocc/PROVA/CMSSW_7_3_0_pre3/src/HLTrigger/special/test/00901F2F-0384-E311-AFE2-002590593902.root'
                                                                        
                                                            
                                                              )
                            
                            #process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False)
                            
                            )

# Production Info
process.configurationMetadata = cms.untracked.PSet(
                                                   version = cms.untracked.string('$Revision: 1.381.2.13 $'),
                                                   annotation = cms.untracked.string('data nevts:500'),
                                                   name = cms.untracked.string('PyReleaseValidation')
                                                  )

#HLTStream

process.hltAlCaPhiSymStream = cms.EDFilter( "HLTEcalPhiSymFilter",
    statusThreshold = cms.uint32( 3 ),
    barrelDigiCollection = cms.InputTag('ecalDigis','ebDigis'),
    endcapDigiCollection = cms.InputTag('ecalDigis','eeDigis'),
    barrelUncalibHitCollection = cms.InputTag("ecalMultiFitUncalibRecHit","EcalUncalibRecHitsEB"),
    endcapUncalibHitCollection = cms.InputTag("ecalMultiFitUncalibRecHit","EcalUncalibRecHitsEE"),
    barrelHitCollection = cms.InputTag('ecalRecHit','EcalRecHitsEB'),
    endcapHitCollection = cms.InputTag('ecalRecHit','EcalRecHitsEE'),
    ampCut_barrel = cms.double(8.),
    ampCut_endcap = cms.double(12.),
    useRecoFlag = cms.bool( False ),
    useConstantThreshold = cms.bool( False ),
    phiSymBarrelDigiCollection = cms.string( "phiSymEcalDigisEB" ),
    phiSymEndcapDigiCollection = cms.string( "phiSymEcalDigisEE" )
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring('drop *',
              'keep *_hltAlCaPhiSymStream_phiSymEcalDigisEB_*',
              'keep *_hltAlCaPhiSymStream_phiSymEcalDigisEE_*',
              'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*' ,
              'keep triggerTriggerEvent_*_*_*' ,
              'keep edmTriggerResults_TriggerResults__HLT' 
    ),
    fileName = cms.untracked.string('Neutrino_Pt-2to20_gun_Fall13dr-tsg_PU40bx50_POSTLS162_V1-v1.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('')
    )
)
              
process.options = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True),
  #SkipEvent = cms.untracked.vstring('FatalRootError','InvalidReference')
)

         
#
# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:com10', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'GR_R_74_V8A::All', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_72_V4A::All', '')

# prefer these conditions over the globalTag's ones
#from CalibCalorimetry.EcalTrivialCondModules.EcalTrivialCondRetriever_cfi import *
#process.load("EcalTrivialCondRetriever_Thresholds_cfi")
#process.addThresholds = EcalTrivialConditionRetriever.clone()
#process.prefer = cms.ESPrefer("EcalTrivialConditionRetriever","addThresholds")

from CondCore.DBCommon.CondDBSetup_cfi import *
process.ecalConditions = cms.ESSource("PoolDBESSource", CondDBSetup,
      connect = cms.string('frontier://FrontierPrep/CMS_COND_ECAL'),
      toGet = cms.VPSet(         # overide Global Tag use EcalTBWeights_EBEE_offline
                  cms.PSet(
                      record = cms.string('EcalPhiSymThresholdsRcd') ,
                      tag = cms.string('EcalPhiSymThresholds_test')
                  )
              )
)
process.prefer = cms.ESPrefer("PoolDBESSource","ecalConditions")

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
#process.reconstruction_step = cms.Path(process.reconstruction*process.hltAlCaPhiSymStream)
process.reconstruction_step = cms.Path(process.ecalLocalRecoSequence*process.hltAlCaPhiSymStream)
#process.hltAlCaPhiSymStream_step = cms.Path(process.hltAlCaPhiSymStream)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)
#process.RECOSIMoutput.outputCommands.append('keep *')

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)

