/*
 *  NTU CSIE BigData Homeowrk #1
 *  PageRank.java
 *  MapReduce code for pagerank.
 *
 *  Qing-Cheng Li (qcl) <r01922024 at csie.ntu.edu.tw>
 *  2013.10.14
 */
package tw.qcl;

import java.io.IOException;
import java.util.*;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;
import org.apache.hadoop.util.*;

public class PageRank {

    public static class Map extends MapReduceBase implements Mapper<LongWritable, Text, Text, Text> {
      private Text word = new Text();

      public void map(LongWritable key, Text value, OutputCollector<Text, Text> output, Reporter reporter) throws IOException {

        String[] splited = value.toString().split("\t");
        String nodeName  = splited[0];
        Double pagerank  = Double.parseDouble(splited[1]);
        String[] outlinks= splited[2].split(" ");
        for(int i=0;i<outlinks.length;i++){
            word.set(outlinks[i]);
            // target, contribute
            output.collect(word,new Text("v"+String.valueOf(pagerank/outlinks.length)));

            // node, outlink
            output.collect(new Text(String.valueOf(nodeName)),word);
        }
      }
    }

    public static class Reduce extends MapReduceBase implements Reducer<Text, Text, Text, Text> {
      public void reduce(Text key, Iterator<Text> values, OutputCollector<Text, Text> output, Reporter reporter) throws IOException {
        
        List<String> outlinks = new ArrayList<String>();
        double sum = 0.0;
        double d = 0.85;
        while (values.hasNext()) {
          String v = values.next().toString();
          if(v.charAt(0)=='v'){
            sum += Double.parseDouble(v.substring(1));
          }else{
            outlinks.add(v);
          }
        }

        sum = 1 - d + d*sum ;

        String out = String.valueOf(sum) + "\t" + outlinks.get(0);
        for(int i=1;i<outlinks.size();i++){
            out = out + " " + outlinks.get(i);
        }
        output.collect(key, new Text(out));
      }
    }

    public static void main(String[] args) throws Exception {

      int iter = Integer.parseInt(args[0]);

      for(int i=0;i<iter;i++){
        JobConf conf = new JobConf(PageRank.class);
        conf.setJobName("qcl-pagerank-iter-"+i);
      
        conf.setOutputKeyClass(Text.class);
        conf.setOutputValueClass(Text.class);
        //conf.setOutputKeyComparatorClass(IntWritable.Comparator.class);
      
        conf.setMapperClass(Map.class);
        conf.setReducerClass(Reduce.class);
      
        conf.setInputFormat(TextInputFormat.class);
        conf.setOutputFormat(TextOutputFormat.class);

        //Set input path
        if(i==0){
            FileInputFormat.setInputPaths(conf, new Path(args[1]));
        }else{
            FileInputFormat.setInputPaths(conf, new Path(args[1]+"-iter-"+(i-1)));
        }

        //Set outout path
        if(i==iter-1){
            FileOutputFormat.setOutputPath(conf, new Path(args[2]));
        }else{
            FileOutputFormat.setOutputPath(conf, new Path(args[1]+"-iter-"+i));
        }

        //run 
        JobClient.runJob(conf);

        //remove tmp input file.
        if(i!=0){
            FileSystem fs = FileSystem.get(conf);
            fs.delete(new Path(args[1]+"-iter-"+(i-1)),true);
        }

      }

    }
}
