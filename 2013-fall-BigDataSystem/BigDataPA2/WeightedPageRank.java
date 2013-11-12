/**
 *  NTU CSIE BigData Homeowrk #2
 *  WeightedPageRank.java
 *  HAMA code for WeightedPageRank.
 *
 *  Qing-Cheng Li (qcl) <r01922024 at csie.ntu.edu.tw>
 *  2013.11.11
 */
package tw.qcl;

import java.io.IOException;
import java.io.DataInput;
import java.io.DataOutput;
import java.util.*;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.Writable;
import org.apache.hama.HamaConfiguration;
import org.apache.hama.bsp.HashPartitioner;
import org.apache.hama.bsp.TextInputFormat;
import org.apache.hama.bsp.TextArrayWritable;
import org.apache.hama.bsp.TextOutputFormat;
import org.apache.hama.graph.AverageAggregator;
import org.apache.hama.graph.Edge;
import org.apache.hama.graph.GraphJob;
import org.apache.hama.graph.Vertex;
import org.apache.hama.graph.VertexInputReader;

public class WeightedPageRank{

    /**
     *  MagicDoubleWritable
     *  This class extends DoubleWritable, store the output weight information
     *  for a vertex and some information for messages
     */
    protected static class MagicDoubleWritable extends DoubleWritable {
       
        private int outLinkNum = 0; //[msg] output link number
        private int inLinkNum = 0;  //[msg] input link number
        private String source = ""; //[msg] where this msg from
        
        //[vertex] output link weight.
        private Map<Text,Double> weight = new HashMap<Text, Double>();
        
        /**
         *  override write function for sending messages
         */
        @Override
        public void write(DataOutput out) throws IOException {
            //super class write
            super.write(out);
            
            //more information write out
            out.writeInt(outLinkNum);
            out.writeInt(inLinkNum);
            out.writeUTF(source);
            
            //write out the map 
            int ws = weight.size();
            out.writeInt(ws);
            if(ws > 0){
                for(Map.Entry<Text, Double> entry: weight.entrySet()){
                    out.writeUTF(entry.getKey().toString());
                    out.writeDouble(entry.getValue());
                }    
            }
        }
        
        /**
         *  override readFields for reading messages
         */
        @Override
        public void readFields(DataInput in) throws IOException {
            //super readFields (DoubleWritable)
            super.readFields(in);
            
            //read back information
            outLinkNum = in.readInt();
            inLinkNum  = in.readInt();
            source     = in.readUTF();
            
            //read back the map
            int ws     = in.readInt();
            if( ws>0 ){
                //rebuild map
                weight = new HashMap<Text, Double>();
                Text key = new Text(in.readUTF());
                double value = in.readDouble();
                weight.put(key,value);
            }
        }

        /**
         *  Constructor
         *  for send/store double information (DoubleWritable)
         */
        public MagicDoubleWritable(double v){
            super(v);
        }

        /**
         *  Constructor
         *  for send/store message of number of input and output link.
         */
        public MagicDoubleWritable(int i, int o){
            super(0.0);
            inLinkNum  = i;
            outLinkNum = o;
        }
        
        /**
         *  Constructor
         */
        public MagicDoubleWritable(){
            super(0.0);
        }
        
        /**
         *  source setter (Text)
         */
        public void setSource(Text s){
            source = s.toString();
        }

        /**
         *  source setter (string)
         */
        public void setSource(String s){
            source = s;
        }
        
        /**
         *  source getter
         */
        public Text getSource(){
            return new Text(source);
        }
        
        /**
         *  input link number getter
         */
        public int getInLinkNum(){
            return inLinkNum;
        }

        /**
         *  input link number setter
         */
        public void setInLinkNum(int i){
            inLinkNum = i;
        }

        /**
         *  output link number getter
         */
        public int getOutLinkNum(){
            return outLinkNum;
        }

        /**
         *  output link number setter
         */
        public void setOutLinkNum(int o){
            outLinkNum = o;
        }

        /**
         *  weight map getter
         */
        public Map<Text, Double> getWeightMap(){
            return weight;
        }

        /**
         *  weight map setter
         */
        public void setWeightMap(Map<Text, Double> map){
            weight = map;
        }
    }

    /**
     *  WeightedPageRank vertex class
     */
    public static class WeightedPageRankVertex extends 
        Vertex<Text, NullWritable, MagicDoubleWritable>{
     
        static double DAMPING_FACTOR = 0.85;
        
        /**
         *  Compute on each vertex
         */
        @Override
        public void compute(Iterable<MagicDoubleWritable> messages) throws IOException{
            
            //Phase 1 - send msg out, let out link know how much input link they have.
            if(this.getSuperstepCount() == 0){
                
                // Init the node value
                MagicDoubleWritable niw = new MagicDoubleWritable(1.0);
                this.setValue(niw);
                
                // Send to all outlink a 1
                MagicDoubleWritable msg = new MagicDoubleWritable(1.0);
                msg.setSource(this.getVertexID());
                sendMessageToNeighbors(msg);
                
            //Phase 2 - count the input link, then send back input and outlink
            }else if(this.getSuperstepCount() == 1){
                
                int out_sum = this.getEdges().size();
                int in_sum = 0;
                
                List<Text> inLinks = new ArrayList<Text>();
               
                //get all input link
                for(MagicDoubleWritable msg: messages){
                    in_sum ++;
                    inLinks.add(msg.getSource());
                }
                
                //send msg (in,out) back
                for(Text t: inLinks){
                    MagicDoubleWritable niw = new MagicDoubleWritable(in_sum,out_sum);
                    niw.setSource(this.getVertexID());
                    sendMessage(t,niw);
                }
                
            //Phase 3 - all vertex know how many in/out their output link have, distribute first run contribution.
            }else if(this.getSuperstepCount() == 2){
                
                int in_sum = 0;
                int out_sum = 0;
                Map<Text, Integer> outOfOut = new HashMap<Text, Integer>();
                Map<Text, Integer> inOfOut  = new HashMap<Text, Integer>();
                Map<Text, Double> weight   = new HashMap<Text, Double>();
                
                //get msg, calculate total input and output number
                for(MagicDoubleWritable msg: messages){
                    int i  = msg.getInLinkNum();
                    int o  = msg.getOutLinkNum();
                    Text link = msg.getSource();
                    
                    in_sum += i;
                    out_sum += o;
                    
                    inOfOut.put(link,i);
                    outOfOut.put(link,o);
                    
                }
               
                //calculate output weight
                for(Map.Entry<Text, Integer> entry: inOfOut.entrySet()){
                    Text k = entry.getKey();
                    int i = entry.getValue();
                    int o = outOfOut.get(k);
                    
                    weight.put(k,(double)(i*o)/(double)(in_sum*out_sum));
                }
                
                //init this vertex
                MagicDoubleWritable niw = this.getValue();
                double v = 1.0/this.getNumVertices();
                niw.setWeightMap(weight);
                niw.set(v);
                this.setValue(niw);
                
                //send out page rank with weight
                for(Map.Entry<Text, Double> entry: this.getValue().getWeightMap().entrySet()){
                    Text   k = entry.getKey();
                    double w = entry.getValue();
                    sendMessage(k,new MagicDoubleWritable(w*v));
                }
                
            //Phase 4 - interation
            }else if(this.getSuperstepCount() >  2){
                //get all input rank, 
                double sum = 0;
                for(MagicDoubleWritable msg : messages){
                    sum += msg.get();
                }
               
                //set the value of pagerank of this vertext
                MagicDoubleWritable niw = this.getValue();
                double alpha = (1.0d - DAMPING_FACTOR);
                double v = alpha + (sum*DAMPING_FACTOR);
                niw.set(v);
                this.setValue(niw);
                
                //send out page rank with weight
                for(Map.Entry<Text, Double> entry: this.getValue().getWeightMap().entrySet()){
                    Text   k = entry.getKey();
                    double w = entry.getValue();
                    sendMessage(k,new MagicDoubleWritable(w*v));
                }
                
            }
            
        }
        
    }

    /**
     *  read input file and parse it.
     */
    public static class WeightedPageRankTextReader 
        extends VertexInputReader<
            LongWritable, 
            Text, 
            Text, 
            NullWritable, 
            MagicDoubleWritable>{
    
        /**
         *  parse vertex and set edge.
         */
        @Override
        public boolean parseVertex(
            LongWritable key, 
            Text value, 
            Vertex<Text, NullWritable, MagicDoubleWritable> vertex) throws Exception{
           
            // parse the input line.
            String[] splited = value.toString().split("\t");
            String vertexName = splited[0];
            String[] outlinks = splited[1].split(" ");
            
            vertex.setVertexID(new Text(vertexName));
            
            for(String outlink: outlinks){
                vertex.addEdge(new Edge<Text, NullWritable>(new Text(outlink),null));
            }
            
            return true;
        }

    }

    /**
     *  create a new graph job
     */
    public static GraphJob createJob(String[] args, HamaConfiguration conf) throws IOException{
    
        GraphJob wprJob = new GraphJob(conf, WeightedPageRank.class);
        wprJob.setJobName("qcl-WeightedPageRank");
        
        wprJob.setVertexClass(WeightedPageRankVertex.class);
        wprJob.setInputPath(new Path(args[1]));
        wprJob.setOutputPath(new Path(args[2]));
        
        wprJob.setMaxIteration(2+Integer.parseInt(args[0]));
        
        if(args.length > 3){
            wprJob.setNumBspTask(Integer.parseInt(args[3]));
        }
        
        wprJob.setAggregatorClass(AverageAggregator.class);
        
        wprJob.setVertexInputReaderClass(WeightedPageRankTextReader.class);
        
        wprJob.setVertexIDClass(Text.class);
        wprJob.setVertexValueClass(MagicDoubleWritable.class);
        wprJob.setEdgeValueClass(NullWritable.class);
        
        wprJob.setInputFormat(TextInputFormat.class);
        
        wprJob.setPartitioner(HashPartitioner.class);
        wprJob.setOutputFormat(TextOutputFormat.class);
        wprJob.setOutputKeyClass(Text.class);
        wprJob.setOutputValueClass(MagicDoubleWritable.class);
        
        return wprJob;

    }

    /**
     *  print how to use this program.
     */
    private static void printUsage(){
        System.out.println("Usage: <iter> <input> <output> [task]");
    }

    /**
     *  main function
     */
    public static void main(String[] args) 
        throws IOException, 
            InterruptedException, 
            ClassNotFoundException{
            
        if(args.length < 3){
            printUsage();
        }else{
            HamaConfiguration conf = new HamaConfiguration(new Configuration());
            GraphJob wprJob = createJob(args, conf);
            
            wprJob.waitForCompletion(true);
        }
        
    }

}

